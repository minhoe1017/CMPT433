
#include "fan.h"
#include "gpio.h"
#include "led.h"


#define VALUE "/sys/class/gpio/gpio%d/value"
#define RELAY "/sys/class/gpio/gpio70/value"


int fanInit(FAN* fan){
	char buff[MAX_LENGTH];
	sprintf(buff, RELAY);

	FILE *pfile = fopen(RELAY, "w");
	if (pfile == NULL) {
		printf("ERROR: Unable to open value file for FAN RELAY.\n");
		exit(1);
	}

	fprintf(pfile, "none");
	fclose(pfile);

	return 0;
}

void fanExport(void){
    FILE *exportfile = fopen(EXPORT, "w");
    if (exportfile == NULL){
        printf("ERROR: Unable to open file.\n");
        exit(1);
    }

    fprintf(exportfile, "%d" , 70);

    fclose(exportfile);
}

int fanOn(FAN* fan){
    return fanWrite(fan, 1);
}

int fanOff(FAN* fan){
    return fanWrite(fan, 0);
}

int fanWrite(FAN* fan, int value){
    char buff[MAX_LENGTH];
    sprintf(buff, RELAY);

    FILE *pfile = fopen(buff, "w");
    if(pfile == NULL ) {
        fprintf(pfile, "Error: Unable to open value file for FAN RELAY.\n");
        exit(1);
    }

    fprintf(pfile, "%d", value);
    fclose(pfile);

    return 0;
}
