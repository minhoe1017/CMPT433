// Demo application to read analog input voltage 0 on the BeagleBone
// Assumes ADC cape already loaded:
// root@beaglebone:/# echo BB-ADC > /sys/devices/platform/bone_capemgr/slots
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "devices.h"

#define A2D_FILE_VOLTAGE0	"/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define A2D_VOLTAGE_REF_V	1.8
#define A2D_MAX_READING		4095


static void Devices_potentiometerConfigure(void) {
  system("echo BB-ADC > /sys/devices/platform/bone_capemgr/slots");
}

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

void Devices_configure(void) {
  Devices_potentiometerConfigure();
  Devices_displayConfigure();
}

static int Devices_getVoltage2piecewiseConversion(int voltage) {
  int result;
  if (voltage < 0 || voltage > 4095) {
    printf("voltage is out of range, something is wrong\n");
    exit(-1);
  }
  else if (voltage == 0) {
    result = 1;
  }
  else if (voltage < 501) {
    result = round(voltage * (20-1)/(500-0) + 1);
  }
  else if (voltage < 1001) {
    result = round(voltage * (60-20)/(1000-500) - 20);
  }
  else if (voltage < 1501) {
    result = round(voltage * (120-60)/(1500-1000) - 60);
  }
  else if (voltage < 2001) {
    result = round(voltage * (250-120)/(2000-1500) - 270);
  }
  else if (voltage < 2501) {
    result = round(voltage * (300-250)/(2500-2000) + 50);
  }
  else if (voltage < 3001) {
    result = round(voltage * (500-300)/(3000-2500) - 700);
  }
  else if (voltage < 3501) {
    result = round(voltage * (800-500)/(3500-3000) - 1300);
  }
  else if (voltage < 4001) {
    result = round(voltage * (1200-800)/(4000-3500) - 2000);
  }
  else {
    result = round(voltage * (2100-1200)/(4100-4000) - 34800);
  }
  return result;
}

static int Devices_getVoltage0Reading() {
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
  return Devices_getVoltage2piecewiseConversion(reading);
}
