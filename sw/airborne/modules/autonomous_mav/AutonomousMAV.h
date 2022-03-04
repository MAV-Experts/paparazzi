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

#include "navigator/Navigator.h"
#include <chrono>
#include "obstacle_detector/ObstacleDetector.h"
#include "firmwares/rotorcraft/navigation.h"
#include "generated/airframe.h"
#include "state.h"
#include "subsystems/abi.h"
#include <time.h>
#include <stdio.h>


#ifndef AUTONOMOUS_MAV_H
#define AUTONOMOUS_MAV_H

// Define the num with the possible states
enum STATE {
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
    STATE currentState;

    /**
     * The navigation unit that does the path planning
     */
    Navigator * navigationUnit;

    /**
     * The obstacle detection unit that the aircraft relies on
     */
    ObstacleDetector * detector;

    /**
     * Time when the system started operating
     */
    std::chrono::steady_clock::time_point operationStarted;

public:

    /**
     * Constructor method of the Autonomous MAV
     */
    void AutonomousMav();

    /**
     * Heartbeat method that is periodically called to run state machine
     */
    void heartbeat();

    /**
     * Method returns whether the aircraft is experiencing an error
     *
     * @return MAV experiencing an error
     */
    bool aircraftHasError();

}

#endif

