#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include "main.h"

static int display;
static _Bool running;
pthread_t thread;
pthread_t thread2;

static void Main_configure(void) {
	Devices_configure();
	UDP_initialize();
	Sorter_startSorting ();
	display = 0;
	running = true;
	/*int iret1;
	iret1 = pthread_create(&thread2, NULL, (void*) Main_runDisplay, NULL);
	if (iret1 != 0) {
		printf("Thread initialization failed!\n");
	}*/
}

/*static void Main_runDisplay(void) {
	while (running) {
		writeNum(i2cInit(), display);
	}
	//writeNum(i2cInit(), 0);
}*/

static void Main_refreshArray(void) {
	char message[100];
	char ssize[50];
	sleep(1);
	int prevCount = Sorter_getNumberArraysSorted();

	while (Sorter_isSorting()) {
		int size = Devices_potentiometerRead();
		Sorter_setArraySize(size);
		printf("Potentiometer sets array size of %d\n", size);
		strcpy(message, "Potentiometer sets array size of ");
		snprintf(ssize, 100 * sizeof(char), "%d", size);
		strcat(message, ssize);
		strcat(message, "\n");
		strcat(message, "\0");
		printf("%s\n", message);


		int count = Sorter_getNumberArraysSorted();
		int countNow = count - prevCount;
		prevCount = count;
		if (countNow > 99)
			countNow = 99;
		int i2cFileDesc = i2cInit();
		writeNum(i2cFileDesc, countNow);
		sleep(1);
	}
	running = false;
}

int main() {
	Main_configure();

	int iret1;
	iret1 = pthread_create(&thread, NULL, (void*) Main_refreshArray, NULL);
	if (iret1 != 0) {
		printf("Thread initialization failed!\n");
	}

	Interface_run();

	return 0;
}
