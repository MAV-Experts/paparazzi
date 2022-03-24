//
// Created by theo on 23-03-22.
//

#include "AutonomousMAV.h"


void heartbeat(struct MAV *self){

    // TODO: perform some crazy state operations

}

int aircraftHasError(struct MAV *self){

    // TODO: check whether the aircraft is experiencing any errors

    return 0;
}

struct MAV createMAV(struct Navigator * nav, struct ObstacleDetector * detector){

    // Create an instance of type MAV
    struct MAV instance;

    // Initialize the attributes of the system
    instance.currentState = STARTED;
    instance.navigator = nav;
    instance.detector = detector;

    // Append all methods to the instance
    instance.heartbeat = heartbeat;
    instance.aircraftHasError = aircraftHasError;

    // Return the instance to the calling block
    return instance;

}