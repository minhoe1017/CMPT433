// Demo application to read analog input voltage 0 on the BeagleBone
// Assumes ADC cape already loaded:
// root@beaglebone:/# echo BB-ADC > /sys/devices/platform/bone_capemgr/slots
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "potentiometer.h"


/*void potentiometer_startThread(){
  pthread_create(&potId, NULL, pot_thread, NULL);
}
void potentiometer_stopThread(){
  pthread_join(potId);
}*/

/*
static void Devices_displayConfigure(void) {
  system("echo 44 > /sys/class/gpio/export");
  system("echo 61 > /sys/class/gpio/export");
  system("echo out > /sys/class/gpio/gpio44/direction");
  system("echo out > /sys/class/gpio/gpio61/direction");
  system("echo 1 > /sys/class/gpio/gpio44/value");
  system("echo 1 > /sys/class/gpio/gpio61/value");
  system("echo BB-I2C1 > /sys/devices/platform/bone_capemgr/slots");
  system("i2cset -y 1 0x20 0x00 0x00");
  system("i2cset -y 1 0x20 0x01 0x00");
}
*/

int Devices_getVoltage2Temp(int voltage) {
  int result;
  if (voltage < 0 || voltage > 4095) {
    printf("voltage is out of range, something is wrong\n");
    exit(-1);
  }
  else {
    //result = round(voltage * (50)/(4095));
    result = (voltage * (50)/(4095));
  }
  return result;
}

int Devices_getVoltage0Reading() {
  // Open file
  FILE *f = fopen(A2D_FILE_VOLTAGE0, "r");
  if (!f) {
	printf("ERROR:	Unable to open voltage input file. Cape loaded?\n");
	printf("try:	echo BB-ADC > /sys/devices/platform/bone_capemgr/slots\n");
	exit(-1);
  }

  // Get reading
  int a2dReading = 0;
  int itemsRead = fscanf(f, "%d", &a2dReading);
  if (itemsRead <= 0) {
	printf("ERROR: Unable to read values from voltage input file.\n");
	exit(-1);
  }

  // Close file
  fclose(f);
  return a2dReading;
}

int Devices_potentiometerRead(void) {
  int reading = Devices_getVoltage0Reading();
  return Devices_getVoltage2Temp(reading);
}

void* potThread(){
    //Devices_potentiometerConfigure();
  // REMEMBER TO SLEEP FOR A SECOND AFTER THIS!!!
    sleep(1);
    while (true) {
    printf("Potentiometer request for temperature is %d\n", Devices_potentiometerRead());
    printf("Waiting 1 second for user input\n");
    sleep(1);
  }

}
