//
// Created by theo on 23-03-22.
//

#ifndef PAPARAZZI_AUTONOMOUSVEHICLE_H
#define PAPARAZZI_AUTONOMOUSVEHICLE_H

#include "navigator/Navigator.h"
#include "obstacle_detector/ObstacleDetector.h"

/**
 * Possible states of the MAV instance
 */
enum STATE {
    STARTED,
    STANDBY,
    NORMAL_MOVEMENT,
    OBSTACLE_FOUND,
    EVASIVE_ACTION,
    DONE
};

/**
 * Define a struct that depicts an object of type MAV
 */
struct MAV {

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
 * TODO:
 *   - One needs to be able to pass a navigator
 *   - One needs to be able to pass an obstacle detector
 *
 * @return
 */
struct MAV createMAV(struct Navigator * nav, struct ObstacleDetector * detector);

#endif //PAPARAZZI_AUTONOMOUSVEHICLE_H
