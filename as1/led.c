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

int LEDon(LED* led);
int LEDoff(LED* led);
int LEDwrite(LED* led, int value);


int LEDinit(LED* led){
	char buff[MAX_LENGTH];
	sprintf(buff, TRIGGER, led -> number);

	FILE *pfile = fopen(TRIGGER, "w");
	if (pfile == NULL) {
		printf("ERROR: Unable to open trigger file for LED %d.\n", led -> number);
		exit(1);
	}

	fprintf(pfile, "none");
	fclose(pfile);

	return 0;
}wd

int LEDflash(LED* led, int times){
    for(int i = 0; i < times; i++) {
    	long seconds = 0.1;
        long nanoseconds = 5000000;
    	struct timespec reqDelay = {seconds, nanoseconds};
        LEDon(led);
        nanosleep(&reqDelay, (struct timespec*) NULL);
        LEDoff(led);
    }
    return 0;
}

int LEDon(LED* led){
	return LEDwrite(led, 1);
}

int LEDoff(LED* led){
	return LEDwrite(led, 0);
}

int LEDwrite(LED* led, int value){
	char buff[MAX_LENGTH];
    sprintf(buff, BRIGHTNESS, led->number);

    FILE *pfile = fopen(buff, "w");
    if(pfile == NULL ) {
        fprintf(pfile, "Error: Unable to open brightness file for LED %d.\n", (led->number));
        exit(1);
    }
    
    fprintf(pfile, "%d", value);
    fclose(pfile);

    return 0;
}

#endif