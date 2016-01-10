#include <bcm2835.h>
#include <stdio.h>

int readadc(int ch) {
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

int main() {
    if (!bcm2835_init()) {
        return 1;
    }

    bcm2835_spi_begin();
    
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

    for (int i = 0; i < 16; i++) {
        printf("%d %d\n", readadc(0), readadc(7));
        delay(250);
    }

    bcm2835_spi_end();
}

