#include <signal.h>
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

static volatile int is_kill = 0;

void siginth(int s) {
    is_kill = 1;
}

int main() {
    // Initialize bcm2835 library
    if (!bcm2835_init()) {
        return 1;
    }

    // Catch Control-C
    signal(SIGINT, siginth);

    // Initialize SPI interface
    bcm2835_spi_begin();
    printf("Initialized\n");
    
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

    // Initialize sensors
    Sensor sl[4] = {
        Sensor(0), // Line 0 (furthest from front)
        Sensor(1), // Line 1
        Sensor(2), // Line 2 (closest to front)
        Sensor(7)  // Pointing forward
    };
    int sensors = sizeof(sl) / sizeof(*sl);

    timespec ts;
    unsigned int ct;

    // Loop
    while (!is_kill) {
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
                }
                else {
                    // Display results
                    s.display_results();
                    fflush(stdout);
                    s.clear();
                }
            }
        }
    }

    // Stop the interface
    printf("Stopped\n");
    bcm2835_spi_end();
}

