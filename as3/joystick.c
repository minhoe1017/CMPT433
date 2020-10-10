#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "joystick.h"
#include "audioMixer.h"


pthread_t threadJoystick;

void nsleep(long sec, long nanosec){
	long seconds = sec;
	long nanoseconds = nanosec;
	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}

FILE* openFile(char* fileName, char* type) {
	FILE *file = fopen(fileName, type);
	if (file == NULL) {
		printf("ERROR: Unable to open file\n");
		exit(-1);
	}

	return file;
}

void closeFile(FILE* file) {
	fclose(file);
}

void exportGpioPin(int pin) {
	FILE *exportFile = openFile(EXPORT, "w");
	int pinWritten = fprintf(exportFile, "%d", pin);
	if (pinWritten <= 0){
		printf("ERROR: Unable to open export file.");
		exit(-1);
	}

	closeFile(exportFile);
}

void writeToFile(char* fileName, char* value) {
	FILE* file = openFile(fileName, "w");
	int charWritten = fprintf(file, "%s", value);
	if (charWritten <= 0) 
		printf("ERROR: Write");

	closeFile(file);
}

void joystickInit(void){
	exportGpioPin(JOYSTICK_VALUE_UP);
	exportGpioPin(JOYSTICK_VALUE_DOWN);
	exportGpioPin(JOYSTICK_VALUE_LEFT);
	exportGpioPin(JOYSTICK_VALUE_RIGHT);
	exportGpioPin(JOYSTICK_VALUE_IN);
}

int readJoystick(char *fileName){
	FILE *file = fopen(fileName, "r");
	if (file == NULL){
		printf("ERROR: Unable to open file (%s) for read\n", fileName);
		exit(-1);
	}
	char buff[MAX_LENGTH];
	fgets(buff, MAX_LENGTH, file);
	
	return atoi(buff);
}

_Bool checkRead(char *fileName){
	int pin = readJoystick(fileName);
	if (pin == 0)
		return 1;
	else
		return 0;
}

void* joystickThread(void* arg) {
	joystickInit();

	int offset = 5;

	while(1) {
		if (checkRead(VALUE_UP)) {
			AudioMixer_setVolume(AudioMixer_getVolume() + offset);
			printf("VOL: %d\n", AudioMixer_getVolume());
		}else if (checkRead(VALUE_DOWN))  {
			AudioMixer_setVolume(AudioMixer_getVolume() - offset);
			printf("VOL: %d\n", AudioMixer_getVolume());
		}else if (checkRead(VALUE_LEFT)) {
			AudioMixer_setBPM(AudioMixer_getBPM() + offset);
			printf("BPM: %d\n", AudioMixer_getBPM());
		}else if (checkRead(VALUE_RIGHT)) {
			AudioMixer_setBPM(AudioMixer_getBPM() - offset);
			printf("BPM: %d\n", AudioMixer_getBPM());
		}else if (checkRead(VALUE_IN)) {
			AudioMixer_nextState();
			printf("State: %d\n", AudioMixer_getState());
		}
		nsleep(0, 100000000);
	}
	pthread_exit(0);
}

void joystickFree(void){
	pthread_join(threadJoystick, NULL);
}
