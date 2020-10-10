

#ifndef _POTENTIOMETER_H_
#define _POTENTIOMETER_H_

#define A2D_FILE_VOLTAGE0	"/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define A2D_VOLTAGE_REF_V	1.8
#define A2D_MAX_READING		4095

//pthread_t potId;

/*void potentiometer_startThread();
void potentiometer_stopThread():*/

int Devices_getVoltage2Temp(int voltage);
int Devices_getVoltage0Reading();
// Should read the potentiometer and return a piecewise value
void Devices_configure(void);
int Devices_potentiometerRead(void);
void *potThread ();
//static void Devices_displayConfigure(void);

#endif
