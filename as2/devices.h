

#ifndef _DEVICES_H_
#define _DEVICES_H_

static void Devices_potentiometerConfigure(void);
static int Devices_getVoltage2piecewiseConversion(int voltage);
static int Devices_getVoltage0Reading();
// Should read the potentiometer and return a piecewise value
void Devices_configure(void);
int Devices_potentiometerRead(void);
 
static void Devices_displayConfigure(void);

#endif 