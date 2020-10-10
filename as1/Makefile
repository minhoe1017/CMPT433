# Makefile for building embedded application.
# by Brian Fraser

OUTFILE = hello
OUTDIR = $(HOME)/cmpt433/public/myApps

CROSS_COMPILE = arm-linux-gnueabihf-
CC_CPP = $(CROSS_COMPILE)g++
CC_C = $(CROSS_COMPILE)gcc

# Indicate using POSIX standard (at least 2008+)
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L


app:
	$(CC_C) $(CFLAGS) hello.c -o $(OUTDIR)/$(OUTFILE)

clean:
	rm $(OUTDIR)/$(OUTFILE)
