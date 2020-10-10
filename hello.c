// DerLightenBlinken game by Brian Fraser
// Print a "hello world" message, and turn on up/down LED
// User guesses using up/down on Joystick.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

// Paths for accessing the sys file system mapping for the LEDs
#define LED_BASE_PATH  "/sys/class/leds/beaglebone:green:usr"
#define LED_TRIGGER    "/trigger"
#define LED_BRIGHTNESS "/brightness"
#define LED_ON "1"
#define LED_OFF "0"	
#define EXPORT_FILE    "/sys/class/gpio/export"

// #define LED_NUM 1
#define NUM_LEDS 4
#define LED_UP 0
#define LED_DOWN 3

#define JOYSTICK_PIN_UP 26
#define JOYSTICK_PIN_DOWN 46
#define JOYSTICK_PIN_LEFT 65
#define JOYSTICK_PIN_RIGHT 47

#define FLASHS_WHEN_PRESSED 3

#define BUFF_SIZE 1024

#define LED_FLASH_TIME_CORRECT_MS 100
#define LED_FLASH_TIME_WRONG_MS 100
#define NUM_FLASHES_WRONG 5


enum DIRECTION {
	DIRECTION_NONE = 0,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT, 
	DIRECTION_RIGHT,
};


static void initializeLEDs();
static void initializeJoystick();
static void playGame();
static void flashAllLeds(unsigned int delayMs);
static void driveAllLeds(char *onOrOff);
static void driveLed(int ledNum, char *on1OrOff0);
static void activateLed(enum DIRECTION target);
static enum DIRECTION getRandomUpOrDown();

static enum DIRECTION getJoystickPress();

static void sleep_ms(unsigned int);

int main() 
{
	printf("Hello embedded world, from Brian!\n");

	printf("\n");
	printf("Press the Zen cape's Joystick in the direction of the LED.\n");
	printf("  UP for LED 0 (top)\n");
	printf("  DOWN for LED 3 (bottom)\n");
	printf("  LEFT/RIGHT for exit app.\n");

	srand(time(NULL));
	initializeLEDs();
	initializeJoystick();

	playGame();
	return 0;
}

static void playGame()
{
	int countCorrect = 0;
	int countTotal = 0;
	while (true) {
		printf("Press joystick; current score (%d / %d)\n", countCorrect, countTotal);
		
		// Set expected
		enum DIRECTION gameChoice = getRandomUpOrDown();
		activateLed(gameChoice);

		// Get user's input
		enum DIRECTION userChoice = DIRECTION_NONE;
		while (userChoice == DIRECTION_NONE) {
			userChoice = getJoystickPress();
		} 

		// Exit?
		if (userChoice == DIRECTION_LEFT || userChoice == DIRECTION_RIGHT) {
			break;
		}

		// Check its
		if (userChoice == gameChoice) {
			printf("Correct!\n");
			flashAllLeds(LED_FLASH_TIME_CORRECT_MS);
			countCorrect++;
		} else {
			printf("Incorrect! :(\n");
			for (int i = 0; i < NUM_FLASHES_WRONG; i++) {
				flashAllLeds(LED_FLASH_TIME_WRONG_MS);
				sleep_ms(LED_FLASH_TIME_WRONG_MS);
			}
		}
		countTotal++;

		// Wait until released
		while (getJoystickPress() != DIRECTION_NONE) {
			// keep waiting
			sleep_ms(10);
		}
	}

	printf("Your final score was (%d / %d)\n", countCorrect, countTotal);
	printf("Thank you for playing!\n");
	driveAllLeds(LED_OFF);
}

static void activateLed(enum DIRECTION target) {
	driveAllLeds(LED_OFF);
	switch (target) {
		case DIRECTION_UP: 
			driveLed(LED_UP, LED_ON);
			break;
		case DIRECTION_DOWN:
			driveLed(LED_DOWN, LED_ON);
			break;
		default: 
			assert(false);
	}
}

static enum DIRECTION getRandomUpOrDown()
{
	if (rand() % 2 == 0) {
		return DIRECTION_UP;
	} else {
		return DIRECTION_DOWN;
	}
}

/******************************************************
 * 				Hardware Initialization
 * ***************************************************/
static void setupLed(int ledNum);
static void initializeJoystick();
static void initializeJoystickDirection(int linuxPinNumber);

static void initializeLEDs()
{
	for (int i = 0; i < NUM_LEDS; i++) {
		setupLed(i);
	}
}
static void setupLed(int ledNum) 
{
	char buff[BUFF_SIZE];
	sprintf(buff, "%s%d%s", LED_BASE_PATH, ledNum, LED_TRIGGER);
	FILE *pTrigger = fopen(buff, "r+");
	if (!pTrigger) {
		printf("Error opening file %s\n", buff);
		exit(1);
	}
	fprintf(pTrigger, "none");
	fclose(pTrigger);
}

static void initializeJoystick()
{
	initializeJoystickDirection(JOYSTICK_PIN_UP);
	initializeJoystickDirection(JOYSTICK_PIN_DOWN);
	initializeJoystickDirection(JOYSTICK_PIN_LEFT);
	initializeJoystickDirection(JOYSTICK_PIN_RIGHT);
}
static void initializeJoystickDirection(int linuxPinNumber)
{
	char buff[BUFF_SIZE];

	// Export Joystick input pin (configure for GPIO):
	FILE *pExportFile = fopen(EXPORT_FILE, "w");
	if (!pExportFile) {
		printf("Error opening file %s\n", EXPORT_FILE);
		exit(1);
	}
	fprintf(pExportFile, "%d", linuxPinNumber);
	fclose(pExportFile);

	// Set direction (input)
	sprintf(buff, "/sys/class/gpio/gpio%d/direction", linuxPinNumber);
	FILE *pDirectionFile = fopen(buff, "w");
	if (!pDirectionFile) {
		printf("Error opening file %s\n", buff);
		exit(1);
	}
	fprintf(pDirectionFile, "in");
	fclose(pDirectionFile);
}



/******************************************************
 * 				Read Joystick
 * ***************************************************/
static int readLineFromFile(char* file_name, char* buff, unsigned int max_length);
static _Bool isJoystickPressInDirection(int joystickPin);

static enum DIRECTION getJoystickPress() 
{
	return 
		isJoystickPressInDirection(JOYSTICK_PIN_UP) ? DIRECTION_UP :
		isJoystickPressInDirection(JOYSTICK_PIN_DOWN) ? DIRECTION_DOWN :
		isJoystickPressInDirection(JOYSTICK_PIN_LEFT) ? DIRECTION_LEFT :
		isJoystickPressInDirection(JOYSTICK_PIN_RIGHT) ? DIRECTION_RIGHT :
		DIRECTION_NONE;
}
static _Bool isJoystickPressInDirection(int joystickPin)
{
	char fileName[BUFF_SIZE];
	sprintf(fileName, "/sys/class/gpio/gpio%d/value", joystickPin);
	char buff[BUFF_SIZE];
	readLineFromFile(fileName, buff, BUFF_SIZE);
	return buff[0] == '0';
}

static int readLineFromFile(char* file_name, char* buff, unsigned int max_length)
{
	FILE *file = fopen(file_name, "r");
	if (file == NULL) {
		printf("FILEIODRV ERROR: Unable to open file for read: %s\n", file_name);
		exit(-1);
	}

	// Read string (line)
	int bytes_read = getline(&buff, &max_length, file);

	// Close
	fclose(file);

	// Null terminate "string" being returned
	buff[bytes_read] = 0;

	return bytes_read;
}




/******************************************************
 * 				LEDs
 * ***************************************************/

static void flashAllLeds(unsigned int delayMs)
{
	driveAllLeds(LED_ON);
	sleep_ms(delayMs);
	driveAllLeds(LED_OFF);
}

static void driveAllLeds(char *onOrOff) 
{
	for (int i = 0; i < NUM_LEDS; i++) {
		driveLed(i, onOrOff);
	}
}

static void driveLed(int ledNum, char *on1OrOff0)
{
	char fileName[BUFF_SIZE];
	sprintf(fileName, "%s%d%s", LED_BASE_PATH, ledNum, LED_BRIGHTNESS);
	FILE *pLedBrightness = fopen(fileName, "w");
	fprintf(pLedBrightness, "%s", on1OrOff0);
	fclose(pLedBrightness);
}


/******************************************************
 * 				Support
 * ***************************************************/
static void sleep_ms(unsigned int delayMs)
{
	const unsigned int NS_PER_MS = 1000 * 1000;
	const unsigned int NS_PER_SECOND = 1000000000;

	unsigned long long delayNs = delayMs * NS_PER_MS;
	int seconds = delayNs / NS_PER_SECOND;
	int nanoseconds = delayNs % NS_PER_SECOND;

	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}

