# Requires root because GPIO

CC=gcc
MAKE=make

CFLAGS=-O3 -std=c++11
LIBS=-l bcm2835 -l rt

.PHONY: all setup clean

all: sensor.x

%.x: %.cpp
	gcc -o $@ $(CFLAGS) $< $(LIBS)

setup:
	cd bcm2835 && ./configure && $(MAKE) && $(MAKE) check && $(MAKE) install

clean:
	-rm sensor.x

