#include "beacon.h"

#include <stdio.h>
#include <bcm2835.h>
#include <time.h>

#define SAMPLES_PER_READ 2
#define MAX_PEAKS 8

class Sensor {
    public:
        int channel;
    
        int detection[7];
        int detection_prev;
        int detection_counter;
    
        int detect_type;
        unsigned int detect_time;
        int peak_index = 0;
        int peak_prev_time[3];
        int peak_prev_intensity[3];
        int peak_time[MAX_PEAKS];
        int peak_intensity[MAX_PEAKS];
        
        Sensor(int ch) : channel(ch) {
            clear();
        }

        void clear() {
            for (int i = 0; i < 7; i++) {
                detection[i] = 0;
            }
            detection_prev = 0;
            detection_counter = 1;
            detect_type = -1;
            peak_index = 0;
            for (int i = 0; i < 3; i++) {
                peak_prev_time[i] = 0;
                peak_prev_intensity[i] = 0;
            }
            for (int i = 0; i < MAX_PEAKS; i++) {
                peak_time[i] = 0;
                peak_intensity[i] = 0;
            }
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
            printf("Ch%d: %d %d %d %d %d %d %d\n", channel,
                detection[0], detection[1], detection[2], detection[3],
                detection[4], detection[5], detection[6]);
            //*/
        }

        void detection_test(unsigned int ct) {
            if (detection_prev == 0 && detection[0] > 3 && detection[0] < 30) {
                // Bounds
                int sub = (detection[0] + detection[1]) / 2 + 4;
                int llb =  detection[0] + detection[1] - 4;
                int lub =  llb * 4;

                // Beacon 0 (Up)
                if (detection[0] <= sub && detection[1] <= sub &&
                    detection[2] <= sub && detection[3] <= sub &&
                    llb <= detection[4] && detection[4] <= lub &&
                    llb <= detection[5] && detection[5] <= lub &&
                    detection[6] <= sub) {
                    detect_type = 0;
                    detect_time = ct;
                    //printf("Detected %d\n", detect_type);
                }
                // Beacon 1 (Down)
                else if (
                    detection[0] <= sub && detection[1] <= sub &&
                    detection[2] <= sub && detection[3] <= sub &&
                    llb <= detection[4] && detection[4] <= lub &&
                    detection[5] <= sub &&
                    llb <= detection[6] && detection[6] <= lub) {
                    detect_type = 1;
                    detect_time = ct;
                    //printf("Detected %d\n", detect_type);
                }
            }
        }

        int read_time(unsigned int ct) {
            return ct - detect_time < LED_TIME_TOTAL + LED_HEADER_DELAY + 10;
        }

        void peak_test(unsigned int ct, int read) {
            peak_prev_time[0] = peak_prev_time[1];
            peak_prev_intensity[0] = peak_prev_intensity[1];
            peak_prev_time[1] = peak_prev_time[2];
            peak_prev_intensity[1] = peak_prev_intensity[2];
            peak_prev_time[2] = ct;
            peak_prev_intensity[2] = read;

            if (peak_index < MAX_PEAKS &&
                peak_prev_intensity[1] > peak_prev_intensity[0] + 4 &&
                peak_prev_intensity[1] + 2 > peak_prev_intensity[2]) {
                peak_intensity[peak_index] = peak_prev_intensity[1];
                peak_time[peak_index] = peak_prev_time[1] - detect_time - LED_HEADER_DELAY;
                peak_index++;
            }
        }

        void display_results() {
            printf("%d %d", channel, detect_type);
            for (int i = 0; i < peak_index; i++) {
                printf("%6d%6d", peak_time[i], peak_intensity[i]);
            }
            printf("\n");
        }
};

