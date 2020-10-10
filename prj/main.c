//PROJECT "FANCONTROLLER"

// Segment display
// # echo BB-I2C1 > /sys/devices/platform/bone_capemgr/slots

// Voltage reading
// # echo BB-ADC > /sys/devices/platform/bone_capemgr/slots

// LED VALUE
// # echo 30 > /sys/class/gpio/export; cd /sys/class/gpio/gpio30; echo out > direction; echo 1 > value
// # echo 60 > /sys/class/gpio/export; cd /sys/class/gpio/gpio60; echo out > direction; echo 1 > value


// FAN VALUE
// # echo 70 > /sys/class/gpio/export; cd /sys/class/gpio/gpio70; echo out > direction; echo 1 > value
// killall node

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

//header files
#include "gpio.h"
#include "udp.h"
#include "led.h"
#include "segDisplay.h"
#include "i2c.h"
#include "tempReader.h"
#include "potentiometer.h"
#include "fan.h"

void* displayThread();
void* ledThread();
void* udpThread();
//void* fanThread();
_Bool compare (char* str1, char* str2, int length);
char stop = ' ';


int main(){
	system("echo BB-I2C1 > /sys/devices/platform/bone_capemgr/slots");
	system("echo BB-ADC > /sys/devices/platform/bone_capemgr/slots");
	system("echo 30 > /sys/class/gpio/export");
	system("echo 60 > /sys/class/gpio/export");
	system("echo 70 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio30/direction");
	system("echo out > /sys/class/gpio/gpio60/direction");
	system("echo out > /sys/class/gpio/gpio70/direction");




	initDisplay();
	UDP_initialize();

	pthread_t displayThr;
	pthread_t ledThr;
	//pthread_t potThr;
	pthread_t udpThr;
//	pthread_t fanThr;
	//pthread_t serverThr;

	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	pthread_create(&displayThr, NULL, displayThread, NULL);
	pthread_create(&ledThr, NULL, ledThread, NULL);
	//pthread_create(&potThr, NULL, potThread, NULL);
	pthread_create(&udpThr, NULL, udpThread, NULL);
//	pthread_create(&fanThr, NULL, fanThread, NULL);
	//pthread_create(&serverThr, &attributes, listen_for_command, NULL);

	pthread_join(displayThr, NULL);
	pthread_join(ledThr, NULL);
	//pthread_join(potThr,NULL);
	pthread_join(udpThr,NULL);
//	pthread_join(fanThr,NULL);
	//pthread_join(serverThr, NULL);
}

void* displayThread() {
	initDisplay();
	int i2cFileDesc = i2cInit();

	while (stop != 'T') {
		int temperature =round(tempRead());

		if (temperature > 99)
			temperature = 99;
		writeNum(i2cFileDesc, temperature);
	}

	printf("Stopped Segment Display Thread\n");
	i2cClose(i2cFileDesc);
	pthread_exit(0);
}

void* ledThread(){
	LED* led = (LED *) malloc(sizeof(LED));
	FAN *fan = (FAN *) malloc(sizeof(FAN));
	gLEDinit(led);
	rLEDinit(led);
	fanInit(fan);
	LEDmain();
	pthread_exit(0);
}
/*
void* fanThread(){
	FAN* fan = (FAN *) malloc(sizeof(FAN));
	fanInit(fan);
	pthread_exit(0);
}
*/
void* udpThread() {
	sleep(1);
	while (true) {
		int readSize = 0;
		//system("netcat -vv localhost 12345 -u &");
		//printf("we are still running during netcat\n");
		char* buffer = UDP_read(&readSize);
		printf("size: %d ---", readSize);
		//UDP_write(buffer);
		char buf[100];
		if (compare(buffer, "pref_temp1", 10)) {
			sprintf(buf, "room1_pref_temp: %d\n", Devices_potentiometerRead());
			UDP_write(buf);
		}
		else if (compare(buffer, "act_temp1", 9)) {
			sprintf(buf, "room1_act_temp: %lf\n", tempRead());
			UDP_write(buf);
		}
	}
}

_Bool compare (char* str1, char* str2, int length) {
	for (int i=0; i<length; i++) {
		if (str1[i] != str2[i]) {
			return false;
		}
	}
	return true;
}
