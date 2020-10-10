// Assumes ADC cape already loaded:
//    root@beaglebone:/#  echo BB-ADC > /sys/devices/platform/bone_capemgr/slots
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "tempReader.h"

int getVoltage1Reading(){
  FILE *f = fopen(A2D_FILE_VOLTAGE1, "r");
  if (!f) {
    printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
    printf("try:   echo BB-ADC > /sys/devices/platform/bone_capemgr/slots\n");
    exit(-1);
  }

  int a2dReading = 0;
  int itemsRead = fscanf(f, "%d", &a2dReading);

  if (itemsRead <= 0) {
    printf("ERROR: Unable to read values from voltage input file.\n");
    exit(-1);
  }

  fclose(f);

  return a2dReading;
}

double CtoF(double c){
  double f = c * (9/5) + 32;
  return f;
}


double tempRead(){
  /*while (true) {
    int reading = getVoltage1Reading();
    double miliVoltage = ((double)reading / A2D_MAX_READING) * A2D_MILIVOLTAGE;
    double celcius = ((double)miliVoltage - 500.0) / 10.0;
    printf("Celsius: %5.5f°C\n", celcius);
    printf("Fahrenheit: %5.5f°F\n", CtoF(celcius));
    sleep(5);
  }*/
  int reading = getVoltage1Reading();
  double miliVoltage = ((double)reading / A2D_MAX_READING) * A2D_MILIVOLTAGE;
  double celcius = ((double)miliVoltage - 500.0) / 10.0;

  return celcius;
}
