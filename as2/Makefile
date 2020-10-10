TARGET = sorter
OBJS = sorter.o interface.o devices.o udp.o main.o segDisplay.o i2c.o
OBJDIR = $(HOME)/cmpt433/public/myApps

CFLAGS = -Wall -g -lm -std=c99 -pthread -D_POSIX_C_SOURCE=199309L 
CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc

all: clean $(TARGET)

$(OBJDIR)/%.o : $(OBJDIR)/%.c
	$(CC) -c $(CFLAGS) $< -lm

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OBJDIR)/$@ -lm

pot: 
	$(CC) $(CFLAGS) -o $(OBJDIR)/pot devices.c -lm

#$(OBJDIR)/devices.o : devices.c
#	$(CC) $(CFLAGS) -o $(OBJDIR)/devices.o devices.c 

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)








#OUTFILE = sorter
#OUTDIR = $(HOME)/cmpt433/public/myApps
#TLIB = $(OUTDIR)/sorter.a
#CROSS_COMPILE = arm-linux-gnueabihf-
#CC_C = $(CROSS_COMPILE)gcc
#CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

#.SUFFIXES: .c

#.c.o:
#	$(CC_C) $(CFLAGS) -c $<

#.c:
#	$(CC_C) $(CFLAGS) $< -o $@ $(TLIB)

#SRC = sorter.c interface.c udp.c potentiometer.c
#OBJ = $(addsuffix .o, $(basename $(SRC)))

#all: app
#help: 
#	@echo "Build sorting module and noworky for beaglebone"
#	@echo "Targets include all, app, and clean."
#app:
#	$(CC_C) $(CFLAGS) -o $@ $(OBJ) $(TLIB)
#	#$(CC_C) $(CFLAGS) helloWorld.c -o $(OUTDIR)/$(OUTFILE)
#clean:
#	rm $(OUTDIR)/$(OUTFILE)




# WRITE:
#	MAKEFILE
#	POTENTIOMETER
#	DISPLAY
#	INTERFACE
#	WRAP UP
#	VALGRIND
#	NOWORKY
#	ETC