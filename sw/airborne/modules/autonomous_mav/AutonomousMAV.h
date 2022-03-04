/*
 * Copyright (C) Group 10
 *
 * TU Delft, 04. Mar 2022
 */
/**
 * @file "modules/autonomous_mav/AutonomousMAV.h"
 * @author Group 10
 *
 * This module represents the brain of a MAV that flies fully autonomous through
 */

#ifndef AUTONOMOUS_MAV_H
#define AUTONOMOUS_MAV_H

// Define the num with the possible states
enum mav_state_t {
    STARTUP,
    FATAL_ERROR,
    TAKEOFF,
    LANDING,
    NORMAL_MOVEMENT,
    EVASIVE_ACTION,
    DONE
};

// Define the flyer class
class AutonomousMav{

private:

    /**
     * Attribute stores the current state of the vehicle
     */
    navigation_state_t currentState;

public:
    /**
     * Constructor method of the Autonomous MAV
     */
    void AutonomousMav();

    /**
     * Heartbeat method that is periodically called to run state machine
     */
    void heartbeat();

}

#endif

