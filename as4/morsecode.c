/*
 * demo_miscdrv.c
 * - Demonstrate how to use a misc driver to easily create a file system link
 *      Author: Brian Fraser
 */
#include <linux/module.h>
#include <linux/miscdevice.h>		// for misc-driver calls.
#include <linux/fs.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <linux/leds.h>
#include <linux/kfifo.h>
//#error Are we building this?

// Morse Code Encodings (from http://en.wikipedia.org/wiki/Morse_code)
//   Encoding created by Brian Fraser. Released under GPL.
//
// Encoding description:
// - msb to be output first, followed by 2nd msb... (left to right)
// - each bit gets one "dot" time.
// - "dashes" are encoded here as being 3 times as long as "dots". Therefore
//   a single dash will be the bits: 111.
// - ignore trailing 0's (once last 1 output, rest of 0's ignored).
// - Space between dashes and dots is one dot time, so is therefore encoded
//   as a 0 bit between two 1 bits.
//
// Example:
//   R = dot   dash   dot       -- Morse code
//     =  1  0 111  0  1        -- 1=LED on, 0=LED off
//     =  1011 101              -- Written together in groups of 4 bits.
//     =  1011 1010 0000 0000   -- Pad with 0's on right to make 16 bits long.
//     =  B    A    0    0      -- Convert to hex digits
//     = 0xBA00                 -- Full hex value (see value in table below)
//
// Between characters, must have 3-dot times (total) of off (0's) (not encoded here)
// Between words, must have 7-dot times (total) of off (0's) (not encoded here).
//
static unsigned short morsecode_codes[] = {
		0xB800,	// A 1011 1    					65  ASCII code (a 97)
		0xEA80,	// B 1110 1010 1
		0xEBA0,	// C 1110 1011 101
		0xEA00,	// D 1110 101
		0x8000,	// E 1
		0xAE80,	// F 1010 1110 1
		0xEE80,	// G 1110 1110 1
		0xAA00,	// H 1010 101
		0xA000,	// I 101
		0xBBB8,	// J 1011 1011 1011 1
		0xEB80,	// K 1110 1011 1
		0xBA80,	// L 1011 1010 1
		0xEE00,	// M 1110 111
		0xE800,	// N 1110 1
		0xEEE0,	// O 1110 1110 111
		0xBBA0,	// P 1011 1011 101
		0xEEB8,	// Q 1110 1110 1011 1
		0xBA00,	// R 1011 101
		0xA800,	// S 1010 1
		0xE000,	// T 111
		0xAE00,	// U 1010 111
		0xAB80,	// V 1010 1011 1
		0xBB80,	// W 1011 1011 1
		0xEAE0,	// X 1110 1010 111
		0xEBB8,	// Y 1110 1011 1011 1
		0xEEA0	// Z 1110 1110 101				90 ASCII code (z 122)
};

#define MY_DEVICE_FILE  "morse-code"

// Define the toy data:
#define START_CHAR 'A'
#define END_CHAR   'z'
#define DATA_SIZE  (END_CHAR - START_CHAR + 1)
#define ASCII -65

static char data[DATA_SIZE];


/******************************************************
 * Parameter
 ******************************************************/
#define DEFAULT_DOTTIME 200
static int dottime = DEFAULT_DOTTIME; // ms

// Declare the variable as a parameter.
//   S_IRUGO makes it's /sys/module node readable.
//   # cat /sys/module/morse-code/parameters/dottime
// Loading:
//   # modinfo morse-code.ko
//   # insmod morse-code.ko dottime=150
module_param(dottime, int, S_IRUGO);



/**************************************************************
 * FIFO Support
 *************************************************************/
// Info on the interface:
//    https://www.kernel.org/doc/htmldocs/kernel-api/kfifo.html#idp10765104
// Good example:
//    http://lxr.free-electrons.com/source/samples/kfifo/bytestream-example.c

#define FIFO_SIZE 256	// Must be a power of 2.
static DECLARE_KFIFO(echo_fifo, char, FIFO_SIZE);



/******************************************************
 * Initialization of Data Function
 ******************************************************/
static void initialize_data(void){
	int i = 0;
	for (i = 0; i < DATA_SIZE; i++) {
		data[i] = i + START_CHAR;
	}
}


/******************************************************
 * LED
 ******************************************************/

DEFINE_LED_TRIGGER(ledtrig);

//Value must be limited to [1 – 2000] (inclusive)
static void dottimeLimit(void){
	if (dottime < 1 || dottime > 2000){
		printk(KERN_INFO "ERROR: dottime value must be limited to [1 – 2000].\nSet to default value.\n");
		dottime = DEFAULT_DOTTIME;
	}
}

static int enqueue(char input){
	if (!kfifo_put(&echo_fifo, input))
		return -EFAULT;
	else
		return 0;

} 

static void my_led_blink_dot(void){
	enqueue('.');
	dottimeLimit();
	led_trigger_event(ledtrig, LED_FULL);
	msleep(dottime);
	led_trigger_event(ledtrig, LED_OFF);
	msleep(dottime);
}

static void my_led_blink_dash(void){
	int dashtime = dottime * 3;
	enqueue('-');
	dottimeLimit();
	led_trigger_event(ledtrig, LED_FULL);
	msleep(dashtime);
	led_trigger_event(ledtrig, LED_OFF);
	msleep(dottime);
}

static my_led_blink(int charToMorse){
	int i;
	int mode = 3; 
	int bytes[16];
	int myBuff[3] = {0};
	int idx = 0;
	
	
	for (i = 15; i >= 0; i--){
		bytes[i] = (morsecode_codes[charToMorse] & (1 << i)) >> i;
		if (bytes[i] == 0){
			if (myBuff[2] == 1){
				my_led_blink_dash();
				mode = 2;
			}else if (myBuff[1] == 1){
				my_led_blink_dot();
				my_led_blink_dot();
				mode = 1;
			}else if (myBuff[0] == 1){
				my_led_blink_dot();
				mode = 0;
			}

			//myBuff[3] = {0};
			idx = 0;
		}else if (bytes[i] == 1){
			myBuff[idx] = 1;
			idx++;		
		}
	}
	return mode;
}



static void led_register(void)
{
	// Setup the trigger's name:
	led_trigger_register_simple("morse-code", &ledtrig);
}

static void led_unregister(void)
{
	// Cleanup
	led_trigger_unregister_simple(ledtrig);
}



/******************************************************
 * Callbacks
 ******************************************************/
static ssize_t my_read(struct file *file, char *buff, size_t count, loff_t *ppos){
	// Pull all available data from fifo into user buffer
	int num_bytes_read = 0;
	if (kfifo_to_user(&echo_fifo, buff, count, &num_bytes_read))
		return -EFAULT;
	
	return num_bytes_read;  // # bytes actually read.
}

static ssize_t my_write(struct file *file, const char *buff, size_t count, loff_t *ppos){
	int i;
	int chToMorse;

	dottimeLimit();

	// Push data into fifo, one byte at a time (with delays)
	for (i = 0; i < count; i++) {
		char ch;
		// Get the character
		if (copy_from_user(&ch, &buff[i], sizeof(ch)))
			return -EFAULT;

		if (ch == ' '){
			if(ch == ' '){
				led_trigger_event(ledtrig, LED_OFF);
				msleep(dottime);
				enqueue(' ');
				enqueue(' ');
				enqueue(' ');
			}
		}else if ((ch >= 65 && ch <= 90)){
			my_led_blink(chToMorse);
			enqueue(' ');
			chToMorse = ASCII;
		}else if((ch >= 97 && ch <= 122)){
			ch = ch - 32;
			chToMorse = chToMorse + ch; 
		}
		
		else if (ch <= ' ')
			continue;

		msleep(100);
	}
	// Return # bytes actually written.
	*ppos += count;
	return count;
}

/******************************************************
 * Misc support
 ******************************************************/
// Callbacks:  (structure defined in /linux/fs.h)
struct file_operations my_fops = {
	.owner    =  THIS_MODULE,
	.read     =  my_read,
	.write    =  my_write,
};

// Character Device info for the Kernel:
static struct miscdevice my_miscdevice = {
		.minor    = MISC_DYNAMIC_MINOR,         // Let the system assign one.
		.name     = MY_DEVICE_FILE,             // /dev/.... file.
		.fops     = &my_fops                    // Callback functions.
};

/******************************************************
 * Driver initialization and exit:
 ******************************************************/
static int __init my_init(void)
{
	int ret;
	printk(KERN_INFO "----> demo_misc driver init(): file /dev/%s.\n", MY_DEVICE_FILE);

	// Register as a misc driver:
	ret = misc_register(&my_miscdevice);

	initialize_data();

	led_register();

	INIT_KFIFO(echo_fifo);

	return ret;
}

static void __exit my_exit(void)
{
	printk(KERN_INFO "<---- demo_misc driver exit().\n");

	// Unregister misc driver
	misc_deregister(&my_miscdevice);

	led_unregister();
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Min Kim_copied code from Brian Fraser");
MODULE_DESCRIPTION("Return some ascii data, and reverse a string to printk.");
MODULE_LICENSE("GPL");