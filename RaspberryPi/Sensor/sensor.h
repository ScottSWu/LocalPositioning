#include "beacon.h"

#include <stdio.h>
#include <bcm2835.h>
#include <time.h>

#define SAMPLES_PER_READ 4

class Sensor {
    public:
        int channel;
    
        int detection[7];
        int detection_prev;
        int detection_counter;
    
        int detect_type;
        unsigned int detect_time;
        int peak_prev;
        int peak_time;
        int peak_intensity;
        
        Sensor(int ch) : channel(ch),
            detection{0, 0, 0, 0, 0, 0, 0}, detection_prev(0), detection_counter(1),
            detect_type(-1), detect_time(0),
            peak_prev(0), peak_time(0), peak_intensity(0) {
        }

        void clear() {
            detection[0] = 0;
            detection[1] = 0;
            detection[2] = 0;
            detection[3] = 0;
            detection[4] = 0;
            detection[5] = 0;
            detection[6] = 0;
            detection_prev = 0;
            detection_counter = 1;
            detect_type = -1;
            peak_prev = 0;
            peak_time = -1;
        }

        void detection_shift(int read) {
            detection[0] = detection[1];
            detection[1] = detection[2];
            detection[2] = detection[3];
            detection[3] = detection[4];
            detection[4] = detection[5];
            detection[5] = detection[6];
            detection[6] = detection_counter;
            detection_prev = read;
            detection_counter = 1;

            /*
            printf("%d %d %d %d %d %d %d\n",
                detection[0], detection[1], detection[2], detection[3],
                detection[4], detection[5], detection[6]);
            */
        }

        void detection_test(unsigned int ct) {
            if (detection_prev == 0 && detection[0] > 3 && detection[0] < 30) {
                // Bounds
                int sub = (detection[0] + detection[1]) / 2 + 4;
                int llb =  detection[0] + detection[1] - 4;

                // Beacon 0 (Up)
                if (detection[0] <= sub && detection[1] <= sub &&
                    detection[2] <= sub && detection[3] <= sub &&
                    llb <= detection[4] && llb <= detection[5] && detection[6] <= sub) {
                    detect_type = 0;
                    detect_time = ct;
                    printf("Detected %d\n", detect_type);
                }
                // Beacon 1 (Down)
                else if (
                    detection[0] <= sub && detection[1] <= sub &&
                    detection[2] <= sub && detection[3] <= sub &&
                    llb <= detection[4] && detection[5] <= sub && llb <= detection[6]) {
                    detect_type = 1;
                    detect_time = ct;
                    printf("Detected %d\n", detect_type);
                }
            }
        }

        int read_time(unsigned int ct) {
            return ct - detect_time < LED_TIME_TOTAL + LED_HEADER_DELAY + 10;
        }

        void peak_test(unsigned int ct, int read) {
            if (peak_prev - 8 > read || read > peak_prev + 8) {
                printf("%d\t%d\n", ct - detect_time - LED_HEADER_DELAY, read);
                peak_prev = read;
            }
        }
};

