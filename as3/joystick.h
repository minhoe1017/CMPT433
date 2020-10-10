#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdio.h>
#define JOYSTICK_VALUE_UP 26
#define JOYSTICK_VALUE_DOWN 46
#define JOYSTICK_VALUE_LEFT 65
#define JOYSTICK_VALUE_RIGHT 47
#define JOYSTICK_VALUE_IN 27

#define EXPORT "/sys/class/gpio/export"
#define VALUE_UP "/sys/class/gpio/gpio26/value"
#define VALUE_DOWN "/sys/class/gpio/gpio46/value"
#define VALUE_LEFT "/sys/class/gpio/gpio65/value"
#define VALUE_RIGHT "/sys/class/gpio/gpio47/value"
#define VALUE_IN "/sys/class/gpio/gpio27/value"

#define MAX_LENGTH 1024

void nsleep(long sec, long nanosec);

FILE* openFile(char* fileName, char* type);
void closeFile(FILE* file);
void exportGPIOFile(int pin);
void writeToFile(char* fileName, char* value);

void joystickInit();
int readJoystick(char *fileName);
_Bool checkRead(char *fileName);

void* joystickThread(void* arg);
void joystickFree(void);

#endif