//
// Created by theo on 23-03-22.
//

#include "navigator/Navigator.h"
#include "obstacle_detector/ObstacleDetector.h"
#include "../../autopilot.h"
#include "../nav/waypoints.h"
#include "generated/airframe.h"
#include "generated/flight_plan.h"
#include "../gps/gps.h"
#include "../../firmwares/rotorcraft/navigation.h"
#include <stdio.h>

#ifndef PAPARAZZI_AUTONOMOUSVEHICLE_H
#define PAPARAZZI_AUTONOMOUSVEHICLE_H

// The dimensions of the matrix that detector and navigator exchange
#define MATRIX_COLS 3
#define MATRIX_ROWS 2

// Number of system iterations until the MAV will take off and start operations
#define MIN_SYSTEM_ITERATIONS_TILL_TAKEOFF 2

// Set this to true when you want to have every method call logged
#define LOG_UNIT_METHOD_CALLS true

// Set this to true when you want to have every system state transition logged
#define LOG_STATE_TRANSITIONS true

/**
 * Possible states of the MAV instance
 */
enum STATE {
    BOOT,
    STARTUP,
    FATAL_ERROR,
    TAKEOFF,
    LANDING,
    NORMAL_MOVEMENT,
    EVASIVE_MOVEMENT,
    REENTER_MOVEMENT,
    DONE
};

/**
 * Define a struct that depicts an object of type MAV
 */
struct MAV {

    /**
     * This variable stores the number of system iterations of the MAV instance
     */
    long iterationCounter;

    /**
     * Attribute that stores the current state of the system
     */
     enum STATE currentState;

     /**
      * This attribute stores the navigation unit that belongs to this MAV
      */
     struct Navigator * navigator;

     /**
      * This attribute stores the obstacle detector unit that belongs to this MAV
      */
     struct ObstacleDetector * detector;

     /**
      * The time when this object started operations
      */
     int timeWhenStarted;

     /**
      * Method reference to the heartbeat function of the system
      *
      * @param self reference to object
      */
     void (*heartbeat)(struct MAV *self);

     /**
      * Method reference to the aircraftHasError function of the system
      *
      * @param self reference to object
      * @return system is okay (0) or has errors (1)
      */
     int (*aircraftHasError)(struct MAV *self);

};

/**
 * A periodically called heartbeat function for the systems operations
 *
 * @param self reference to object
 */
void heartbeat(struct MAV *self);

/**
 * This method checks the aircraft's systems status
 *
 * @param self reference to object
 * @return has error (1) or none (0)
 */
int aircraftHasError(struct MAV *self);

/**
 * This function creates an object of type MAV and returns it
 *
 * @param nav reference to navigation unit
 * @param detector reference to detector unit
 * @return an instance of a MAV
 */
struct MAV createMAV(struct Navigator * nav, struct ObstacleDetector * detector);

#endif //PAPARAZZI_AUTONOMOUSVEHICLE_H
