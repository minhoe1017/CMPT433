# Makefile for building multi-file application
# by Brian Fraser

# final executable to build:
TARGET= fanControl
# .o files to link in (for all .c files)
SOURCES = main.c gpio.c segDisplay.c i2c.c tempReader.c led.c potentiometer.c fan.c udp.c

PUBDIR = $(HOME)/cmpt433/public/myApps

# Add support for pthread and real-time clock here:
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror


# You should not need to change any of this:
CC_C = arm-linux-gnueabihf-gcc -pthread

all: clean node
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(PUBDIR)/$(TARGET)  -lpthread -lm

node:
	mkdir -p $(PUBDIR)/server/
	cp -R Server/* $(PUBDIR)/server/ 

clean:
	rm -f $(PUBDIR)/$(TARGET)
