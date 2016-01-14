#include "sensor.h"

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
    // Initialize bcm2835 library
    if (!bcm2835_init()) {
        return 1;
    }

    // Initialize SPI interface
    bcm2835_spi_begin();
    
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

    // Initialize sensors
    Sensor sl[2] = {
        Sensor(0), // Facing down
        Sensor(7)  // Facing up
    };
    int sensors = sizeof(sl) / sizeof(*sl);

    timespec ts;
    unsigned int ct;

    // Loop
    while (!kill) {
        // For each sensor
        for (int i = 0; i < sensors; i++) {
            Sensor &s = sl[i];
            if (s.detect_type < 0) { // Detection state
                int read = read_adc(s.channel) > 40;
                if (read == s.detection_prev) {
                    s.detection_counter++;
                }
                else { // Edge
                    s.detection_shift(read);

                    clock_gettime(CLOCK_MONOTONIC, &ts);
                    ct = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
                    s.detection_test(ct);
                }
            }
            else { // Reading state
                clock_gettime(CLOCK_MONOTONIC, &ts);
                ct = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
                if (s.read_time(ct)) {
                    int read = read_adc(s.channel);
                    // Filter out noise
                    if (read > 30) {
                        s.peak_test(ct, read);
                    }
                    else {
                        s.peak_prev = 0;
                    }
                }
                else {
                    printf("%d %d %3d %3d\n", s.channel, s.detect_type, s.peak_time, s.peak_intensity);
                    fflush(stdout);
                    s.clear();
                }
            }
        }
    }

    // Stop the interface
    bcm2835_spi_end();
}

