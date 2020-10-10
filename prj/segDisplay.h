#ifndef SEGDISPLAY_H_
#define SEGDISPLAY_H_

#define LEFT_DIGIT_DIR "/sys/class/gpio/gpio61/direction"
#define RIGHT_DIGIT_DIR "/sys/class/gpio/gpio44/direction"
#define LEFT_DIGIT_VAL "/sys/class/gpio/gpio61/value"
#define RIGHT_DIGIT_VAL "/sys/class/gpio/gpio44/value"

#define EXPORT "/sys/class/gpio/export"

void initDisplay();
void leftNumFunc();
void rightNumFunc();

FILE* openFile(char* fileName, char* type);
void closeFile(FILE* file);
void exportGpioPin(int pin);
void writeToFile(char* fileName, char* value);

#endif