#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "i2c.h"
#include "segDisplay.h"

void nsleep(long sec, long nanosec){
	long seconds = sec;
	long nanoseconds = nanosec;
	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}

int i2cInit(void){
	int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
	writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
	writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);
	writeI2cReg(i2cFileDesc, REG_OUTA, 0x00);
	writeI2cReg(i2cFileDesc, REG_OUTB, 0x00);

	return i2cFileDesc;
}

void i2cClose(int i2cFileDesc){
	close(i2cFileDesc);
}

int initI2cBus(char* bus, int address){
	int i2cFileDesc = open(bus, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(1);
	}

	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if (result < 0) {
		perror("I2C: Unable to set I2C device to slave address.");
		exit(1);
	}
	return i2cFileDesc;
}

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value){
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("I2C: Unable to write i2c register.");
		exit(1);
	}
}

unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr){
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("I2C: Unable to write to i2c register.");
		exit(1);
	}

	// Now read the value and return it
	char value = 0;
	res = read(i2cFileDesc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("I2C: Unable to read from i2c register");
		exit(1);
	}
	return value;
}

void writeNum(int i2cFileDesc, int number){
	if (number > 99) 
		number = 99;

	int leftNum = (int) (number / 10);
	int rightNum = number % 10;

	int* leftDigit = getNumDisplay(leftNum);
	int* rightDigit = getNumDisplay(rightNum);
	
	leftNumFunc();
	writeI2cReg(i2cFileDesc, REG_OUTA, leftDigit[0]);
	writeI2cReg(i2cFileDesc, REG_OUTB, leftDigit[1]);
	nsleep(0, 5000000);

	rightNumFunc();
	writeI2cReg(i2cFileDesc, REG_OUTA, rightDigit[0]);
	writeI2cReg(i2cFileDesc, REG_OUTB, rightDigit[1]);
	nsleep(0, 5000000);

	free(leftDigit);
	free(rightDigit);
}

/*#define ZERO_L  0xA1
#define ZERO_U  0x86
#define ONE_L   0x80
#define ONE_U   0x02
#define TWO_L   0x31
#define TWO_U   0x0E
#define THREE_L 0xB0
#define THREE_U 0x0E
#define FOUR_L  0x90
#define FOUR_U  0x8A
#define FIVE_L  0xB0
#define FIVE_U  0x8C
#define SIX_L   0xB1
#define SIX_U   0x8C
#define SEVEN_L 0x80
#define SEVEN_U 0x06
#define EIGHT_L 0xB1
#define EIGHT_U 0x8E
#define NINE_L  0x90
#define NINE_U  0x8E*/
int* getNumDisplay(int number) {
	int* numDisplay = (int*) malloc(2 * sizeof(int));
	switch(number) {
		case 0:
			numDisplay[0] = 0xA1;
			numDisplay[1] = 0x86;
      			break;
		case 1:
			numDisplay[0] = 0x80;
			numDisplay[1] = 0x02;
      			break;
		case 2:
			numDisplay[0] = 0x31;
			numDisplay[1] = 0x0E;
      			break;
		case 3:
			numDisplay[0] = 0xB0;
			numDisplay[1] = 0x0E;
      			break;
		case 4:
			numDisplay[0] = 0x90;
			numDisplay[1] = 0x8A;
      			break;
		case 5:
			numDisplay[0] = 0xB0;
			numDisplay[1] = 0x8C;
      			break;
		case 6:
			numDisplay[0] = 0xB1;
			numDisplay[1] = 0x8C;
      			break;
		case 7:
			numDisplay[0] = 0x80;
			numDisplay[1] = 0x06;
      			break;
		case 8:
			numDisplay[0] = 0xB1;
			numDisplay[1] = 0x8E;
      			break;
		case 9:
			numDisplay[0] = 0x90;
			numDisplay[1] = 0x8E;
      			break;
		default:
			numDisplay[0] = 0xA1;
			numDisplay[1] = 0x86;
	}
	return numDisplay;
}
