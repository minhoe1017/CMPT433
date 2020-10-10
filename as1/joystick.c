#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <stdio.h>
#include <stdlib.h>

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

void joystick_init(void){
	FILE *exportfile = fopen(EXPORT, "w");
	if (exportfile == NULL){
		printf("ERROR: Unable to open file.\n");
		exit(1);
	}

	fprintf(exportfile, "%d" , up);
	fprintf(exportfile, "%d" , down);
	fprintf(exportfile, "%d" , left);
	fprintf(exportfile, "%d" , right);
	//fprintf(exportfile, "%d" , centre);

	fclose(exportfile);
}

_Bool gpioRead(int pin){
	char buff[MAX_LENGTH];
	sprintf(buff, VALUE, pin);

	FILE *file = fopen(buff,"r");
	if (file == NULL) {
		printf("ERROR: Unable to open file for read\n");
		exit(-1);
	}
	char* retVal = (char *) malloc(sizeof(char));

	fgets(retVal, MAX_LENGTH, file);

	fclose(file);
	int val = (int)(*retVal);
	if (val == 49) return 0;
	else return 1;
}

int joystick_direction(PIN* dir){
	char buff[MAX_LENGTH];
	sprintf(buff, VALUE, dir -> pinNum);

	FILE *file = fopen(buff, "r");
	if (file == NULL){
		printf("ERROR: Unable to open file (%s) for read\n", buff);
		exit(-1);
	}

	fgets(buff, MAX_LENGTH, file);
	
	return atoi(buff);
}


#endif