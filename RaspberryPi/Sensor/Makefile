# Requires root because GPIO

CC=gcc
MAKE=make

CFLAGS=-O3 -std=c++11 -I ../Beacon
LIBS=-l bcm2835 -l rt

.PHONY: all setup clean

all: sensor.x

%.x: %.cpp %.h
	gcc -o $@ $(CFLAGS) $< $(LIBS)

setup:
	cd bcm2835 && ./configure && $(MAKE) && $(MAKE) check && $(MAKE) install

run:
	sudo ./sensor.x

clean:
	-rm sensor.x

