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

// Create an instance of the autonomous mav (global)
AutonomousMav * mav;

/**
 * Function that is called once for initialization
 */
void runner_init(void){
    // Create an instance for the MAV
    mav = new AutonomousMav();
}

/**
 * Function that is called periodically (for FSM logic)
 */
void runner_periodic(void){
    // Pass heartbeat to the system
    mav->heartbeat();
    // Return for finishing routine
    return;
}