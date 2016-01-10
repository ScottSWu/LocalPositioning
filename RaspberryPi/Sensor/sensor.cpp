#define SAMPLES_PER_READ 8

#include "sensor.hpp"
#include <bcm2835.h>
#include <stdio.h>
#include <time.h>

int read_raw_adc(int ch) {
    char xfer[3] = {1, 0, 0};
    char recv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    xfer[1] = (8 + ch) << 4;
    bcm2835_spi_transfernb(xfer, recv, 3);

    /*
    printf("Ch %d: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        ch, recv[0], recv[1], recv[2], recv[3],
        recv[4], recv[5], recv[6], recv[7]);
    */

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
        int detect = 0;
        int s0 = 0;
        int s1 = 0;
        int s2 = 0;
        int s3 = 0;
        int s4 = 0;
        int s5 = 0;
        int s6 = 0;
        int s7 = 0;
        int prev = 0;
        int prevc = 1;
        while (!detect) { // Detection stage
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
                s6 = s7;
                s7 = prevc;

                // A - 1010 110010
                // B - 1010 110100
                // Detection
                if (prev == 0 && s0 > 3 && s0 < 30) {
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
                    }
                    printf("%d %d %d %d %d %d %d %d\n", s0, s1, s2, s3, s4, s5, s6, s7);
                }

                // Reset counter
                prev = read;
                prevc = 1;
            }
        }
        printf("Detected!\n");
    }

    bcm2835_spi_end();
}

