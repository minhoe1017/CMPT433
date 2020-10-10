# Makefile for building multi-file application
# by Brian Fraser

# final executable to build:
TARGET= hello
# .o files to link in (for all .c files)
OBJS = hello.o led.o joystick.o 

# Add support for pthread and real-time clock here:
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror 


# You should not need to change any of this:
CC_C = arm-linux-gnueabihf-gcc

all: clean $(TARGET) 
	cp hello $(HOME)/cmpt433/public/myApps/
	

%.o : %.c
	$(CC_C) $(CFLAGS) -c $<

$(TARGET): $(OBJS)
	$(CC_C) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)