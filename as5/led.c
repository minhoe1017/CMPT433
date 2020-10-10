// GPIO LED demo
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"
#include "hw_types.h"      // For HWREG(...) macro
#include "watchdog.h"
#include "led.h"

/*****************************************************************************
 **                INTERNAL MACRO DEFINITIONS
 *****************************************************************************/
#define LED_GPIO_BASE           (SOC_GPIO_1_REGS)
#define LED0_PIN (21)
#define LED1_PIN (22)
#define LED2_PIN (23)
#define LED3_PIN (24)

#define LED_MASK ((1<<LED0_PIN) | (1<<LED1_PIN) | (1<<LED2_PIN) | (1<<LED3_PIN))

//#define DELAY_TIME 0x4000000		// Delay with MMU enabled
#define DELAY_TIME 0x40000		// Delay witouth MMU and cache

void initializeLeds(void);
void barPattern(void);
void bouncePattern(void);

static void busyWait(unsigned int count);


/*****************************************************************************
 **                INTERNAL FUNCTION DEFINITIONS
 *****************************************************************************/


void initializeLeds(void)
{
	/* Enabling functional clocks for GPIO1 instance. */
	GPIO1ModuleClkConfig();

	/* Selecting GPIO1[23] pin for use. */
	//GPIO1Pin23PinMuxSetup();

	/* Enabling the GPIO module. */
	GPIOModuleEnable(LED_GPIO_BASE);

	/* Resetting the GPIO module. */
	GPIOModuleReset(LED_GPIO_BASE);

	/* Setting the GPIO pin as an output pin. */
	GPIODirModeSet(LED_GPIO_BASE,
			LED0_PIN,
			GPIO_DIR_OUTPUT);
	GPIODirModeSet(LED_GPIO_BASE,
			LED1_PIN,
			GPIO_DIR_OUTPUT);
	GPIODirModeSet(LED_GPIO_BASE,
			LED2_PIN,
			GPIO_DIR_OUTPUT);
	GPIODirModeSet(LED_GPIO_BASE,
			LED3_PIN,
			GPIO_DIR_OUTPUT);
}

/*
 ** The main function. Application starts here.
 */
/*int main()
{
	initializeLeds();

	driveLedsWithSWFunction();
	driveLedsWithSetAndClear();
	driveLedsWithBitTwiddling();
}*/


/*Bar: Turn on multiple LEDs, as needed, such that a bar of LEDs appears to grow and
shrink. For one time-period each, turn on LEDs 0 through N (inclusive), where
N = 0, 1, 2, 3, 2, 1, 0, 1, 2, 3, 2, 1, 0, ….*/
void barPattern(void)								// Not sure about this funcion
{
	int wdCounter = 0;

	/*// Set all the LEDs:
		HWREG(LED_GPIO_BASE + GPIO_SETDATAOUT) = LED_MASK;
		busyWait(DELAY_TIME);

		// Clear all the LEDs:
		HWREG(LED_GPIO_BASE + GPIO_CLEARDATAOUT) = LED_MASK;
		busyWait(DELAY_TIME);*/

	while(1)
	{
		for (int pin = LED0_PIN; pin <= LED3_PIN; pin++) {
			/* Driving a logic HIGH on the GPIO pin. */
			GPIOPinWrite(LED_GPIO_BASE,
					pin,
					GPIO_PIN_HIGH);

			busyWait(DELAY_TIME);
		}

		HWREG(LED_GPIO_BASE + GPIO_CLEARDATAOUT) = LED_MASK;
		busyWait(DELAY_TIME);


		// Hit the watchdog (must #include "watchdog.h"
		// Each time you hit the WD, must pass it a different number
		// than the last time you hit it.
		wdCounter++;
		WatchdogTimerTriggerSet(SOC_WDT_1_REGS, wdCounter);
	}
}

/*Bounce: Turning on one LED at a time, make the lit LED appear to bounce back
and forth. The illumination sequence will be (one time-period each):
0, 1, 2, 3, 2, 1, 0, 1, 2, 3, 2, 1, 0, ….*/
void bouncePattern(void)
{
	int wdCounter = 0;

	// Clear all the LEDs:
	HWREG(LED_GPIO_BASE + GPIO_CLEARDATAOUT) = LED_MASK;
	busyWait(DELAY_TIME);

	while(1)
	{
		// Flash each LED individually in 0-3
		for (int pin = LED0_PIN; pin <= LED3_PIN; pin++) {
			/* Driving a logic HIGH on the GPIO pin. */
			GPIOPinWrite(LED_GPIO_BASE,
					pin,
					GPIO_PIN_HIGH);

			busyWait(DELAY_TIME);

			/* Driving a logic LOW on the GPIO pin. */
			GPIOPinWrite(LED_GPIO_BASE,
					pin,
					GPIO_PIN_LOW);

			busyWait(DELAY_TIME);
		}

		// Flash each LED individually in 3-0
		for (int pin1 = LED3_PIN; pin1 >= LED0_PIN; pin1--) {
			/* Driving a logic HIGH on the GPIO pin. */
			GPIOPinWrite(LED_GPIO_BASE,
					pin1,
					GPIO_PIN_HIGH);

			busyWait(DELAY_TIME);

			/* Driving a logic LOW on the GPIO pin. */
			GPIOPinWrite(LED_GPIO_BASE,
					pin1,
					GPIO_PIN_LOW);

			busyWait(DELAY_TIME);
		}

		// Hit the watchdog (must #include "watchdog.h"
		// Each time you hit the WD, must pass it a different number
		// than the last time you hit it.
		wdCounter++;
		WatchdogTimerTriggerSet(SOC_WDT_1_REGS, wdCounter);
	}
}

/*
 ** Busy-wait function
 */
static void busyWait(volatile unsigned int count)
{
	while(count--)
		;
}

