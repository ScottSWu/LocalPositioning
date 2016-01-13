# Testing the gpio pins and infrared sensor/emitter
# Following https://learn.adafruit.com/reading-a-analog-in-and-controlling-audio-volume-with-the-raspberry-pi/script

import os, time
import spidev

spi = spidev.SpiDev()

spi.open(0, 0)

CH_LEFT = 0
CH_RIGHT = 7

def read_adc(ch):
    total = 0
    for i in range(4):
        r = spi.xfer2([1, (8+ch)<<4, 0])
        out = ((r[1]&3) << 8) + r[2]
        total = total + out
    return int(total / 4)

def main():
    while True:
        print "{0:4d} {1:4d} {2:4d} {3:4d} {4:4d} {5:4d} {6:4d} {7:4d}".format(
                read_adc(0), read_adc(1), read_adc(2), read_adc(3),
                read_adc(4), read_adc(5), read_adc(6), read_adc(7))
        #lread = read_adc(CH_LEFT)
        #rread = read_adc(CH_RIGHT)
        #print "Left: {0:4d}    Right: {1:4d}".format(lread, rread)
        time.sleep(0.25)

main()

