#ifndef _FAN_H_
#define _FAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define VALUE "/sys/class/gpio/gpio%d/value"
#define RELAY "/sys/class/gpio/gpio70/value"
#define MAX_LENGTH 1024

enum fanStatus{fanon, fanoff};

typedef enum fanStatus fan_status;

typedef struct {
    int number;
    fan_status stat;
} FAN;

int fanInit(FAN* fan);
void fanExport(void);

int fanOn(FAN* fan);
int fanOff(FAN* fan);

int fanWrite(FAN* fan, int value);

#endif
