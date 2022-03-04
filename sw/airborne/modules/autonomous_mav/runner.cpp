/*
 * Copyright (C) Group 10
 *
 * TU Delft, 04. Mar 2022
 */
/**
 * @file "modules/autonomous_mav/runner.cpp"
 * @author Group 10
 *
 * This is the core of the greatest autonomous flight module every created in paparazzi. The architecture is built in
 * a finite state machine, that has two inflight states "Normal Movement" and "Evasive Maneuver" in which two main
 * modules are working: Navigator and ObstacleDetector.
 *
 */

#include "runner.h"

// Create an instance of the autonomous mav
AutonomousFlyer mav;

/**
 * Function that is called once for initialization
 */
void runner_init(void){
    // Create an instance for the MAV
    mav = AutonomousFlyer();
}

/**
 * Function that is called periodically (for FSM logic)
 */
void runner_periodic(void){
    // Pass heartbeat to the system
    mav.heartbeat();
    // Return for finishing routine
    return;
}