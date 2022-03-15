#ifndef AUTONOMOUS_MAV
#define AUTONOMOUS_MAV

#include "navigator/Navigator.h"
#include "obstacle_detector/ObstacleDetector.h"

/**
 * Possible states of the MAV instance
 */
enum STATE {
    STARTUP,
    FATAL_ERROR,
    TAKEOFF,
    LANDING,
    NORMAL_MOVEMENT,
    EVASIVE_ACTION,
    DONE
};

/**
 * Define a struct that depicts an object of type MAV
 *
 */
struct MAV {

    /**
     * Attribute stores the current state of the vehicle
     */
    enum STATE currentState;

    /**
     * The navigation unit that does the path planning
     */
    struct Navigator * navigator;

    /**
     * The obstacle detection unit that the aircraft relies on
     */
    struct ObstacleDetector * detector;

    /**
     * Time when the system started operating
     *
     * TODO: implement some smart way of measuring the time
     */
    int timeWhenStarted;

    /**
     * Heartbeat method that is periodically called to run state machine
     */
    void (*heartbeat)(struct MAV *self);

    /**
     * Method returns whether the aircraft is experiencing an error
     *
     * @return MAV experiencing an error
     */
    int (*aircraftHasError)(struct MAV *self);

};

#endif