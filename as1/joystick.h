#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <stdio.h>

#define EXPORT "/sys/class/gpio/export"
//#define UNEXPORT "/sys/class/gpio/unexport"
#define VALUE "/sys/class/gpio/gpio%d/value"
#define DIRECTION "sys/class/gpio/gpio%d/direction"
#define MAX_LENGTH 1024

enum DIRECTIONSTATUS{centre = 0, up = 26, down = 46, left = 65, right = 47};

typedef enum DIRECTIONSTATUS directionstatus;

typedef struct {
    int pinNum;
    directionstatus dirstat;
} PIN;

void joystick_init(void);
int joystick_direction(PIN* dir);
_Bool gpioRead(int pin);

#endif