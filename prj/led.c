#include "led.h"
#include "gpio.h"
#include "tempReader.h"
#include "potentiometer.h"
#include "fan.h"


int LEDmain();

int gLEDinit(LED* led){
	char buff[MAX_LENGTH];
	sprintf(buff, GREENVALUE);//was GREENVALUE

	FILE *pfile = fopen(GREENVALUE, "w"); //was GREENVSLUE
	if (pfile == NULL) {
		printf("ERROR: Unable to open value file for GREEN LED.\n");
		exit(1);
	}

	fprintf(pfile, "none");
	fclose(pfile);

	return 0;
}

int rLEDinit(LED* led){
	char buff[MAX_LENGTH];
	sprintf(buff, REDVALUE);

	FILE *pfile = fopen(REDVALUE, "w");
	if (pfile == NULL) {
		printf("ERROR: Unable to open value file for RED LED.\n");
		exit(1);
	}

	fprintf(pfile, "none");
	fclose(pfile);

	return 0;
}

void LEDexport(void){
    FILE *exportfile = fopen(EXPORT, "w");
    if (exportfile == NULL){
        printf("ERROR: Unable to open file.\n");
        exit(1);
    }

    fprintf(exportfile, "%d" , green);
    fprintf(exportfile, "%d" , red);

    fclose(exportfile);
}

int gLEDon(LED* led){
    return gLEDwrite(led, 1);
}

int rLEDon(LED* led){
    return rLEDwrite(led, 1);
}

int gLEDoff(LED* led){
    return gLEDwrite(led, 0);
}

int rLEDoff(LED* led){
    return rLEDwrite(led, 0);
}

int LEDflash(LED* led, int times){
    for(int i = 0; i < times; i++) {
    	long seconds = 0.1;
        long nanoseconds = 5000000;
    	struct timespec reqDelay = {seconds, nanoseconds};
        gLEDon(led);
        nanosleep(&reqDelay, (struct timespec*) NULL);
        gLEDoff(led);
    }
    return 0;
}


int gLEDwrite(LED* led, int value){
    char buff[MAX_LENGTH];
    sprintf(buff, GREENVALUE);

    FILE *pfile = fopen(buff, "w");
    if(pfile == NULL ) {
        fprintf(pfile, "Error: Unable to open value file for GREEN LED.\n");
        exit(1);
    }

    fprintf(pfile, "%d", value);
    fclose(pfile);

    return 0;
}

int rLEDwrite(LED* led, int value){
    char buff[MAX_LENGTH];
    sprintf(buff, REDVALUE);

    FILE *pfile = fopen(buff, "w");
    if(pfile == NULL ) {
        fprintf(pfile, "Error: Unable to open value file for RED LED.\n");
        exit(1);
    }

    fprintf(pfile, "%d", value);
    fclose(pfile);

    return 0;
}

int LEDmain(){
    LED* gled = (LED *) malloc(sizeof(LED));
    LED* rled = (LED *) malloc(sizeof(LED));
    FAN* fan = (FAN *) malloc(sizeof(FAN));

    //fanInit(fan);
    //int userTemp = Devices_potentiometerRead();
    int currentTemp = tempRead();


    do{
        //gLEDoff(gled);
        //rLEDoff(rled);
				//fanOff(fan);
				//system("echo 0 > /sys/class/gpio/gpio70/value");

        if (Devices_potentiometerRead() < currentTemp){                            // RED
            fanOn(fan);
					//	printf("testing fan function here........");
						//system("echo 1 > /sys/class/gpio/gpio70/value");
            gLEDoff(gled);
	    	    rLEDoff(rled);
            rLEDon(rled);


	    	currentTemp = tempRead();
        }else if(Devices_potentiometerRead() > currentTemp){                       // GREEN
            fanOff(fan);
						//system("echo 0 > /sys/class/gpio/gpio70/value");
            rLEDoff(rled);
	    	    gLEDoff(gled);
            gLEDon(gled);

	    	currentTemp = tempRead();
        }else if(Devices_potentiometerRead() == currentTemp){                      // userTemp == currentTemp
            fanOff(fan);
						//system("echo 0 > /sys/class/gpio/gpio70/value");

            gLEDoff(gled);
            rLEDoff(rled);

            LEDflash(gled, 1);
	    		LEDflash(rled, 1);

	    	currentTemp = tempRead();
        }
		currentTemp = tempRead();

    } while(true);

    free(rled);
    free(gled);
    free(fan);
    return 0;
}
