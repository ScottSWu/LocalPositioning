# Requires root because GPIO

CC=gcc
MAKE=make

CFLAGS=-O3
LIBS=-l bcm2835

.PHONY: all setup clean

all: sensor.x

%.x: %.c
	gcc -o $@ $(CFLAGS) $< $(LIBS)

setup:
	cd bcm2835 && ./configure && $(MAKE) && $(MAKE) check && $(MAKE) install

clean:
	-rm sensor.x

