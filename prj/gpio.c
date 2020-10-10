
#include "gpio.h"


void gpio_init(void){
	FILE *exportfile = fopen(EXPORT, "w");
	if (exportfile == NULL){
		printf("ERROR: Unable to open file.\n");
		exit(1);
	}

	// joystick control
	fprintf(exportfile, "%d" , up);
	fprintf(exportfile, "%d" , down);
	fprintf(exportfile, "%d" , left);
	fprintf(exportfile, "%d" , right);

	// LED control
	fprintf(exportfile, "%d" , green);
	fprintf(exportfile, "%d" , red);

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