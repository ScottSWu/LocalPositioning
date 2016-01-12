// Onboard light for the Raspberry Pi B+
#define LED_ONBOARD_PORT    GPIO47_PORT
#define LED_ONBOARD_PAD     GPIO47_PAD

// First set of LED lights, counter clockwise
#define LED_0_0_PORT    GPIO17_PORT
#define LED_0_0_PAD     GPIO17_PAD
#define LED_0_1_PORT    GPIO27_PORT
#define LED_0_1_PAD     GPIO27_PAD
#define LED_0_2_PORT    GPIO22_PORT
#define LED_0_2_PAD     GPIO22_PAD
#define LED_0_3_PORT    GPIO10_PORT
#define LED_0_3_PAD     GPIO10_PAD
#define LED_0_4_PORT    GPIO9_PORT
#define LED_0_4_PAD     GPIO9_PAD
#define LED_0_5_PORT    GPIO18_PORT
#define LED_0_5_PAD     GPIO18_PAD
#define LED_0_6_PORT    GPIO23_PORT
#define LED_0_6_PAD     GPIO23_PAD
#define LED_0_7_PORT    GPIO24_PORT
#define LED_0_7_PAD     GPIO24_PAD
#define LED_0_8_PORT    GPIO25_PORT
#define LED_0_8_PAD     GPIO25_PAD

// Second set of LED lights, counter clockwise
#define LED_1_0_PORT    GPIO5_PORT
#define LED_1_0_PAD     GPIO5_PAD
#define LED_1_1_PORT    GPIO6_PORT
#define LED_1_1_PAD     GPIO6_PAD
#define LED_1_2_PORT    GPIO13_PORT
#define LED_1_2_PAD     GPIO13_PAD
#define LED_1_3_PORT    GPIO19_PORT
#define LED_1_3_PAD     GPIO19_PAD
#define LED_1_4_PORT    GPIO26_PORT
#define LED_1_4_PAD     GPIO26_PAD
#define LED_1_5_PORT    GPIO12_PORT
#define LED_1_5_PAD     GPIO12_PAD
#define LED_1_6_PORT    GPIO16_PORT
#define LED_1_6_PAD     GPIO16_PAD
#define LED_1_7_PORT    GPIO20_PORT
#define LED_1_7_PAD     GPIO20_PAD
#define LED_1_8_PORT    GPIO21_PORT
#define LED_1_8_PAD     GPIO21_PAD

// LED functions
#define LED_ON(t,d)     palSetPad(LED_ ## t ## _ ## d ## _PORT, LED_ ## t ## _ ## d ## _PAD)
#define LED_OFF(t,d)    palClearPad(LED_ ## t ## _ ## d ## _PORT, LED_ ## t ## _ ## d ## _PAD)
#define LED_ALLON(t)    LED_ON(t,0); LED_ON(t,1); LED_ON(t,2); LED_ON(t,3); LED_ON(t,4); LED_ON(t,5); LED_ON(t,6); LED_ON(t,7); LED_ON(t,8)
#define LED_ALLOFF(t)   LED_OFF(t,0); LED_OFF(t,1); LED_OFF(t,2); LED_OFF(t,3); LED_OFF(t,4); LED_OFF(t,5); LED_OFF(t,6); LED_OFF(t,7); LED_OFF(t,8)

// Timings (milliseconds for now)
// Header code timings (total time is 10 * SHORT)
#define LED_HEADER_SHORT    5
#define LED_HEADER_LONG     10
// Delay after the header
#define LED_HEADER_DELAY    50

// Per LED time on (total time is (ON + OFF) * 9 + DELAY)
#define LED_TIME_ON         20
// Per LED time off
#define LED_TIME_OFF        20

// Delay after LEDs
#define LED_TIME_DELAY      40

// Everything adds up to 1000 now
#define LED_HEADER_TOTAL    50
#define LED_TIME_TOTAL      360
#define LED_TOTAL           500

// Timing functions
#define SYNC_INIT()     unsigned long ss, se, st
#define SYNC_WAIT(t)    ss = halGetCounterValue(); se = halGetCounterValue(); st = t * 1000; while (se - ss < st) { se = halGetCounterValue(); }
