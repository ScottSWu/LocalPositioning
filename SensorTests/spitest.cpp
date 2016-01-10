#define SAMPLES 20000

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
    const int avg = 8;
    int total = 0;
    for (int i = 0; i < avg; i++) {
        total += read_raw_adc(ch);
    }
    return int(total / avg);
}

int main() {
    if (!bcm2835_init()) {
        return 1;
    }

    bcm2835_spi_begin();
    
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

    int values[SAMPLES];
    timespec times[SAMPLES];
    for (int i = 0; i < SAMPLES; i++) {
        values[i] = read_adc(7);
        clock_gettime(CLOCK_MONOTONIC, &times[i]);
    }

    for (int i = 0; i < SAMPLES; i++) {
        printf("%d%09d %d\n", times[i].tv_sec, times[i].tv_nsec, values[i]);
    }

    bcm2835_spi_end();
}

