README.txt file for Smart Temperature Regulator.

included:
	All source files (.c, .h)
		- fan
		- gpio
		- i2c
		- led
		- potentiometer
		- segDisplay
		- tempReader
		- udp
		- main.c
	Server folder (.js, .html, .css)
	this README.txt
	Makefile

Wiring circuit:
	Check the How to guide for fan and relay connection
	Also, check Writeup for other components	

Pre-compiler:
	Mounting target to host:
		# ./mountNFS	
	
	Segment display:
		# echo BB-I2C1 > /sys/devices/platform/bone_capemgr/slots 
	Voltage reading:
		# echo BB-ADC > /sys/devices/platform/bone_capemgr/slots
	LED VALUE:
		# echo 30 > /sys/class/gpio/export; cd /sys/class/gpio/gpio30; echo out > direction; echo 1 > value
		# echo 60 > /sys/class/gpio/export; cd /sys/class/gpio/gpio60; echo out > direction; echo 1 > value
	FAN VALUE:
		# echo 70 > /sys/class/gpio/export; cd /sys/class/gpio/gpio70; echo out > direction; echo 1 > value

How to build:
	# cd /mnt/remote/myApps/server
	# node server.js & ./../fanControl

Website address:
	192.168.7.2:3434

To stop:
	# ctrl + c
