#ifndef _TEMPREADER_H_
#define _TEMPREADER_H_


#define A2D_FILE_VOLTAGE1  "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
//#define A2D_VOLTAGE_REF_V  1.8
#define A2D_MILIVOLTAGE    1800 // 1.8 * 1000
#define A2D_MAX_READING    4095


int getVoltage1Reading();
double CtoF(double c);
double tempRead();

#endif
