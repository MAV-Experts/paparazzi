//
// Created by theo on 24-03-22.
//

#include "BasicNavigator.h"

enum NAV_STATES getState(struct Navigator *self){

    // Log status update of navigator
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicNavigator: getState()\n");

    return NAV_NORMAL;
}

void computePath(struct Navigator *self, struct DATA_MATRIX obstacleMap){

    // Log status update of navigator
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicNavigator: computePath()\n");

    // TODO: use the obstacleMap to compute an amazing next waypoint

}

int hasError(struct Navigator *self){

    // Log status update of navigator
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicNavigator: hasError()\n");

    return 0;
}

void start(struct Navigator *self){

    // Log status update of navigator
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicNavigator: start()\n");

    // Set the ground reference for the navigator
    NavSetGroundReferenceHere();

}

void stop(struct Navigator *self){

    // Log status update of navigator
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicNavigator: stop()\n");

    // Stop any kind of throttle
    NavKillThrottle();

    // TODO: implement
}

void land(struct Navigator *self){

    // Log status update of navigator
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicNavigator: land()\n");

    // Check whether the autopilot is in flight mode
    if(autopilot_in_flight()){

        // Set current location as the waypoint to be at
        waypoint_set_here_2d(WP_TRAJECTORY);

        // TODO: make this more normal later, for now just turn off engines
        autopilot_set_motors_on(false);

    }

}

void takeoff(struct Navigator *self){

    // Log status update of navigator
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicNavigator: takeoff()\n");

    // Check if the autopilot is not in flight mode already
    if(!autopilot_in_flight()){

        // Turn on motors of aircraft
        NavResurrect();

        // TODO: in orange avoid, they just set the waypoint WP_CLIMB
        waypoint_set_here_2d(WP_CLIMB);

        // Set waypoint to the autopilot
        waypoint_move_xy_i(WP_CLIMB, 0, 0);
        waypoint_set_alt_i(WP_CLIMB, 2);

        // Log successful take off and autopilot mode
        fprintf(stderr, "SUCCESS: Autopilot started (mode %d)\n", autopilot_get_mode());

    } else {

        // Log error that autopilot is already in flight mode
        fprintf(stderr, "WARNING: Autopilot is already in flight mode\n");

    }

}

struct Navigator createBasicNavigator(){
    // Create a navigator instance
    struct Navigator instance;

    // Initialize the core attributes
    instance.currentState = NAV_NORMAL;

    // Append the function references to the object
    instance.getState = getState;
    instance.computePath = computePath;
    instance.hasError = hasError;
    instance.start = start;
    instance.stop = stop;
    instance.takeoff = takeoff;
    instance.land = land;

    // Return the instance to the calling unit
    return instance;
}
