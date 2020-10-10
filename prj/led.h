#ifndef _LED_H_
#define _LED_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define VALUE "/sys/class/gpio/gpio%d/value"
#define REDVALUE "/sys/class/gpio/gpio30/value"
#define GREENVALUE "/sys/class/gpio/gpio60/value"
#define RELAY "/sys/class/gpio/gpio70/value"
#define MAX_LENGTH 1024

enum LEDStatus{on, off};

typedef enum LEDStatus led_status;

typedef struct {
    int number;
    led_status stat;
} LED;

int gLEDinit(LED* led);
int rLEDinit(LED* led);

int LEDflash(LED* led, int times);

int gLEDon(LED* led);
int rLEDoff(LED* led);
int gLEDon(LED*gled);
int rLEDoff(LED* led);

int gLEDwrite(LED* led, int value);
int rLEDwrite(LED* led, int value);

int LEDmain();

#endif
