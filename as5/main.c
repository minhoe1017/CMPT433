// On the serial port, fakes

#include "consoleUtils.h"
#include <stdint.h>

// My hardware abstraction modules
#include "serial.h"
#include "timer.h"
#include "wdtimer.h"
#include "led.h"
#include "joystick.h"
#include "fakeTyper.h"

/******************************************************************************
 **              SERIAL PORT HANDLING
 ******************************************************************************/
static volatile uint8_t s_rxByte = 0;
static void serialRxIsrCallback(uint8_t rxByte) {
	s_rxByte = rxByte;
}

static void doBackgroundSerialWork(void)
{
	if (s_rxByte != 0) {
		if (s_rxByte == '?') {
			ConsoleUtilsPrintf("?\t: Display this help message.\n\
				0-9\t: Set speed 0 (slow) to 0 (fast).\n\
				a\t: Select pattern A (bounce).\n\
				b\t: Select pattern B (bar).\n\
				x\t: Stop hitting the watchdog.\n\
				B\t: Push-button to toggle mode.\n");
		}else if (s_rxByte == 'a') {
			ConsoleUtilsPrintf("Changing to bounce mode.\n");
			bouncePattern();
		}else if (s_rxByte == 'b') {
			ConsoleUtilsPrintf("Changing to bar mode.\n");
			barPattern();									// Is this right function?
		}else if (s_rxByte == 'x') {
			ConsoleUtilsPrintf("No longer hitting the watchdog.\n");
			// Stop Watchdog here.
			// WatchdogTimerDisable(SOC_WDT_1_REGS);
			Watchdog_stop();
		}else if (s_rxByte == 'B') {
			ConsoleUtilsPrintf("Chaning to toggle mode.\n\
				Try joystick on Zencape.\n");
			joystickMain();
		}else {
			ConsoleUtilsPrintf("Invalid command.\n");
		}

		s_rxByte = 0;
	}
}




/******************************************************************************
 **              MAIN
 ******************************************************************************/
int main(void)
{
	// Initialization
	Serial_init(serialRxIsrCallback);
	Timer_init();
	Watchdog_init();
	FakeTyper_init();
	initializeLeds();
	initializeJoystick();

	// Setup callbacks from hardware abstraction modules to application:
	Serial_setRxIsrCallback(serialRxIsrCallback);
	Timer_setTimerIsrCallback(FakeTyper_notifyOnTimeIsr);
	Timer_setTimerIsrCallback(joystick_notifyOnTimeIsr);

	// Display startup messages to console:
	ConsoleUtilsPrintf("LightBouncer:\n\
		\tby Minhoe Kim and Ken Ni\n\
		----------------------------\n\
		Reset source (0x1) = Cold reset,\n\n");
	ConsoleUtilsPrintf("?\t: Display this help message.\n\
				0-9\t: Set speed 0 (slow) to 0 (fast).\n\
				a\t: Select pattern A (bounce).\n\
				b\t: Select pattern B (bar).\n\
				x\t: Stop hitting the watchdog.\n\
				BTN\t: Push-button to toggle mode.\n");

	// Main loop:
	while(1) {
		// Handle background processing
		doBackgroundSerialWork();
		FakeTyper_doBackgroundWork();
		//joystickMain();

		// Timer ISR signals intermittent background activity.
		if(Timer_isIsrFlagSet()) {
			Timer_clearIsrFlag();
			Watchdog_hit();
		}
	}
}
