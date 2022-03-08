/*
 * Copyright (C) Group 10
 *
 * TU Delft, 04. Mar 2022
 */
/**
 * @file "modules/autonomous_mav/runner.cpp"
 * @author Group 10
 *
 * This is runner that "hosts" an object of type AutonomousMAV and passes the heartbeat with a fixed period.
 *
 * TODO:
 *  - We could subscribe to ABI messaging service here and pass information to MAV object
 *
 */

#include "runner.h"
#include "AutonomousMAV.h"
#include "navigator/modules/BasicNavigator.h"
#include "obstacle_detector/modules/BasicDetector.h"

// Create an instance of the autonomous mav (global)
AutonomousMav * mav;

/**
 * Function that is called once for initialization
 */
void runner_init(void){

    // Create a basic navigator and detector
    Navigator * nav = new BasicNavigator();
    ObstacleDetector * detector = new BasicDetector();

    // Create an instance for the MAV
    mav = new AutonomousMav( nav, detector );
}

/**
 * Function that is called periodically (for FSM logic)
 */
void runner_periodic(void){
    // Pass heartbeat to the system
    mav->heartbeat();
    // Return for finishing routine
    return 0;
}