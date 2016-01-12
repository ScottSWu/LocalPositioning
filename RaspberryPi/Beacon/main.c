/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "beacon.h"
#include "ch.h"
#include "hal.h"
#include "test.h"

static WORKING_AREA(waThread1, 32);
static msg_t Thread1(void *p) {
    (void)p;
    chRegSetThreadName("indicator");
    while (TRUE) {
        // Turn on
        palSetPad(LED_ONBOARD_PORT, LED_ONBOARD_PAD);
        chThdSleepMilliseconds(500);
        // Turn off
        palClearPad(LED_ONBOARD_PORT, LED_ONBOARD_PAD);
        chThdSleepMilliseconds(500);
    }
    return 0;
}

static WORKING_AREA(waThread2, 128);
static msg_t Thread2(void *p) {
    (void)p;
    chRegSetThreadName("beacon");
    SYNC_INIT();
    while (TRUE) {
        // First set of LEDs
        // Header
        LED_ALLON(0);  SYNC_WAIT(LED_HEADER_SHORT);
        LED_ALLOFF(0); SYNC_WAIT(LED_HEADER_SHORT);
        LED_ALLON(0);  SYNC_WAIT(LED_HEADER_SHORT);
        LED_ALLOFF(0); SYNC_WAIT(LED_HEADER_SHORT);
        
        LED_ALLON(0);  SYNC_WAIT(LED_HEADER_LONG);
        LED_ALLOFF(0); SYNC_WAIT(LED_HEADER_LONG);
        LED_ALLON(0);  SYNC_WAIT(LED_HEADER_SHORT);
        LED_ALLOFF(0); SYNC_WAIT(LED_HEADER_SHORT);
        
        SYNC_WAIT(LED_HEADER_DELAY);
        
        // LEDs
        // For the RPi port, this is essentially microseconds.
        LED_ON(0, 0);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(0, 0); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(0, 1);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(0, 1); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(0, 2);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(0, 2); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(0, 3);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(0, 3); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(0, 4);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(0, 4); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(0, 5);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(0, 5); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(0, 6);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(0, 6); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(0, 7);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(0, 7); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(0, 8);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(0, 8); SYNC_WAIT(LED_TIME_OFF);
        
        // Final pause
        SYNC_WAIT(LED_TIME_DELAY);
        
        // Second set of LEDs
        // Header
        LED_ALLON(1);  SYNC_WAIT(LED_HEADER_SHORT);
        LED_ALLOFF(1); SYNC_WAIT(LED_HEADER_SHORT);
        LED_ALLON(1);  SYNC_WAIT(LED_HEADER_SHORT);
        LED_ALLOFF(1); SYNC_WAIT(LED_HEADER_SHORT);
        
        LED_ALLON(1);  SYNC_WAIT(LED_HEADER_LONG);
        LED_ALLOFF(1); SYNC_WAIT(LED_HEADER_SHORT);
        LED_ALLON(1);  SYNC_WAIT(LED_HEADER_LONG);
        LED_ALLOFF(1); SYNC_WAIT(LED_HEADER_SHORT);
        
        SYNC_WAIT(LED_HEADER_DELAY);
        
        // LEDs
        // For the RPi port, this is essentially microseconds.
        LED_ON(1, 0);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(1, 0); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(1, 1);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(1, 1); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(1, 2);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(1, 2); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(1, 3);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(1, 3); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(1, 4);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(1, 4); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(1, 5);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(1, 5); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(1, 6);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(1, 6); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(1, 7);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(1, 7); SYNC_WAIT(LED_TIME_OFF);
        LED_ON(1, 8);  SYNC_WAIT(LED_TIME_ON);
        LED_OFF(1, 8); SYNC_WAIT(LED_TIME_OFF);
        
        // Final pause
        SYNC_WAIT(LED_TIME_DELAY);
    }
    return 0;
}

/*
 * Application entry point.
 */
int main(void) {
    // Initialize
    halInit();
    chSysInit();

    // Set pin modes
    palSetPadMode(LED_ONBOARD_PORT, LED_ONBOARD_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_0_0_PORT, LED_0_0_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_0_1_PORT, LED_0_1_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_0_2_PORT, LED_0_2_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_0_3_PORT, LED_0_3_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_0_4_PORT, LED_0_4_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_0_5_PORT, LED_0_5_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_0_6_PORT, LED_0_6_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_0_7_PORT, LED_0_7_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_0_8_PORT, LED_0_8_PAD, PAL_MODE_OUTPUT);
    /*
    palSetPadMode(LED_1_0_PORT, LED_1_0_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_1_1_PORT, LED_1_1_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_1_2_PORT, LED_1_2_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_1_3_PORT, LED_1_3_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_1_4_PORT, LED_1_4_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_1_5_PORT, LED_1_5_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_1_6_PORT, LED_1_6_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_1_7_PORT, LED_1_7_PAD, PAL_MODE_OUTPUT);
    palSetPadMode(LED_1_8_PORT, LED_1_8_PAD, PAL_MODE_OUTPUT);
    */

    // Run indicator
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
    chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

    // Events servicing loop.
    chThdWait(chThdSelf());

    return 0;
}
