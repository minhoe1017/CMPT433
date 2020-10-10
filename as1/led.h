#ifndef _LED_H_
#define _LED_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRIGGER "/sys/class/leds/beaglebone:green:usr%d/trigger" //
#define BRIGHTNESS "/sys/class/leds/beaglebone:green:usr%d/brightness"
#define MAX_LENGTH 1024

enum LEDStatus{on, off};

typedef enum LEDStatus led_status;

typedef struct {
    int number;
    led_status stat;
} LED;

int LEDinit(LED* led);
int LEDflash(LED* led, int times);
int LEDon(LED* led);
int LEDoff(LED* led);
int LEDwrite(LED* led, int value);


#endif