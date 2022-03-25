//
// Created by theo on 23-03-22.
//

#include "runner.h"

/**
 * Create a global instance of the Micro Air Vehicle instance
 */
struct MAV mav;
bool isRunning = false;

/**
 * Create global instances of the navigator and the obstacle detector
 */
struct Navigator nav;
struct ObstacleDetector detector;

/**
 * This will switch the global running variable "on"
 */
extern void startSystem(void){
    isRunning = true;
}

/**
 * This will switch the global running variable "on"
 */
extern void stopSystem(void){
    isRunning = false;
}

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
    if(isRunning){
        mav.heartbeat(&mav);
    }

}