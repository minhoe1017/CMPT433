#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "segDisplay.h"

#define LEFT_DIGIT_DIR "/sys/class/gpio/gpio61/direction"
#define RIGHT_DIGIT_DIR "/sys/class/gpio/gpio44/direction"
#define LEFT_DIGIT_VAL "/sys/class/gpio/gpio61/value"
#define RIGHT_DIGIT_VAL "/sys/class/gpio/gpio44/value"


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

void initDisplay() {
	exportGpioPin(61);
	exportGpioPin(44);
	writeToFile(LEFT_DIGIT_DIR, "out");
	writeToFile(RIGHT_DIGIT_DIR, "out");
}

void leftNumFunc() {
	writeToFile(LEFT_DIGIT_VAL, "1");
	writeToFile(RIGHT_DIGIT_VAL, "0");
}

void rightNumFunc() {
	writeToFile(RIGHT_DIGIT_VAL, "1");
	writeToFile(LEFT_DIGIT_VAL, "0");
}

void exportGpioPin(int pin) {
	FILE *exportFile = openFile(EXPORT, "w");
	int pinWritten = fprintf(exportFile, "%d", pin);
	if (pinWritten <= 0) 
		printf("ERROR: Export");

	closeFile(exportFile);
}

void writeToFile(char* fileName, char* value) {
	FILE* file = openFile(fileName, "w");
	int charWritten = fprintf(file, "%s", value);
	if (charWritten <= 0) 
		printf("ERROR: Write");

	closeFile(file);
}