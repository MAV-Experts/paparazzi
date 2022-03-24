//
// Created by theo on 23-03-22.
//

#include "runner.h"

float oa_color_count_frac = 0.18f;

/**
 * Create a global instance of the Micro Air Vehicle instance
 */
struct MAV mav;

/**
 * Create global instances of the navigator and the obstacle detector
 */
struct Navigator nav;
struct ObstacleDetector detector;

/**
 * Init function in Paparazzi loop
 */
extern void autonomous_mav_init(void){

    // Print status message for user
    fprintf(stderr, "Initializing Autonomous MAV Instances ...\n");

    // Create navigator and detector
    detector = createBasicDetector();
    nav = createBasicNavigator();

    // Create an instance of a MAV object
    mav = createMAV(&nav, &detector);

}

/**
 * Periodic function in Paparazzi loop (called with specific frequency, see xml file)
 */
extern void autonomous_mav_periodic(void){

    // Call the heartbeat method on the instance of the MAV
    mav.heartbeat(&mav);

}