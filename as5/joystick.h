#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_


void initializeJoystick(void);
//static _Bool readButtonWithStarteWare(void);
//static _Bool readButtonWithBitTwiddling(void);
void joystick_notifyOnTimeIsr(void);
void joystickMain();

//static void uartInitialize(void);
//static void uartBaudRateSet(void);

#endif