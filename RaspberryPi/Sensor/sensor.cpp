#define SAMPLES_PER_READ 8

#include "sensor.h"
#include "beacon.h"
#include <bcm2835.h>
#include <stdio.h>
#include <time.h>

int read_raw_adc(int ch) {
    char xfer[3] = {1, 0, 0};
    char recv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    xfer[1] = (8 + ch) << 4;
    bcm2835_spi_transfernb(xfer, recv, 3);

    return ((recv[1] & 3) << 8) + recv[2];
}

int read_adc(int ch) {
    int total = 0;
    for (int i = 0; i < SAMPLES_PER_READ; i++) {
        total += read_raw_adc(ch);
    }
    return int(total / SAMPLES_PER_READ);
}

static volatile int kill = 0;

void siginth(int t) {
    kill = 1;
}

int main() {
    if (!bcm2835_init()) {
        return 1;
    }

    bcm2835_spi_begin();
    
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

    while (!kill) {
        // Timing variables
        timespec ts;
        int tmr, tme;

        // Detection stage
        int detect = 0;
        int s0 = 0;
        int s1 = 0;
        int s2 = 0;
        int s3 = 0;
        int s4 = 0;
        int s5 = 0;
        int s6 = 0;
        int prev = 0;
        int prevc = 1;
        while (!detect) {
            int read = read_adc(7) > 40;
            if (read == prev) { // Still running
                prevc++;
            }
            else { // Edge
                // Shift runs down
                s0 = s1;
                s1 = s2;
                s2 = s3;
                s3 = s4;
                s4 = s5;
                s5 = s6;
                s6 = prevc;

                // A - 1010 110010
                // B - 1010 110100
                // Detection
                if (prev == 1 && s0 > 3 && s0 < 30) {
                    // Bounds
                    int sub = (s0 + s1) / 2 + 4;
                    int llb = s0 + s1 - 4;
                    if (s0 <= sub &&
                        s1 <= sub &&
                        s2 <= sub &&
                        s3 <= sub &&
                        llb <= s4 &&
                        llb <= s5 &&
                        s6 <= sub) {
                        detect = 1;
                        clock_gettime(CLOCK_MONOTONIC, &ts);
                        tmr = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
                    }
                    //printf("%d %d %d %d %d %d %d\n", s0, s1, s2, s3, s4, s5, s6);
                }

                // Reset counter
                prev = read;
                prevc = 1;
            }
        }
        printf("Detected!\n");
        // Reading stage
        prev = 0;
        tme = tmr;
        while (tme - tmr < LED_TOTAL - LED_TIME_TOTAL - LED_HEADER_DELAY) {
            // Get the current time
            clock_gettime(CLOCK_MONOTONIC, &ts);
            tme = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
            int read = read_adc(7);
            // Filter out noise
            if (read > 30) {
                // Make sure this is a new peak
                if (prev - 8 > read || read > prev + 8) {
                    printf("%d\t%d\n", tme - tmr - LED_HEADER_DELAY, read);
                    prev = read;
                }
            }
            else {
                prev = 0;
            }
        }
    }

    bcm2835_spi_end();
}

