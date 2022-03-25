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

    // Set the autopilot mode to GUIDED
    autopilot_mode_auto2 = AP_MODE_GUIDED;
    autopilot_static_set_mode(AP_MODE_GUIDED);

    // TODO: use the obstacleMap to compute an amazing next waypoint

    // Set forward speed
    guidance_h_set_guided_body_vel(12, 0);

    // Set a heading
    guidance_h_set_guided_heading(52);

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

    // Set the altitude reference to here
    NavSetAltitudeReferenceHere();

}

void stop(struct Navigator *self){

    // Log status update of navigator
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicNavigator: stop()\n");

    // Stop any kind of throttle
    NavKillThrottle();

    // Set the autopilot mode back to NAV
    autopilot_mode_auto2 = AP_MODE_NAV;
    autopilot_set_mode(AP_MODE_NAV);

    // TODO: implement
}

void land(struct Navigator *self) {

    // Log status update of navigator
    if (LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicNavigator: land()\n");

    // Check whether the autopilot is in flight mode
    if (autopilot_in_flight()) {

        // Set the autopilot mode back to NAV
        autopilot_mode_auto2 = AP_MODE_NAV;
        autopilot_set_mode(AP_MODE_NAV);

        // Set guidance mode
        guidance_h_mode_changed(GUIDANCE_H_MODE_GUIDED);

        // Set current location as the waypoint to be at TODO: guided?
        NavSetWaypointHere(WP_TD);

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

        // Start engines
        NavResurrect();

        // Set v speed
        nav_climb_vspeed = 10;

        // Set current location as the waypoint to be at
        NavSetWaypointHere(WP_CLIMB);

        // Go to this waypoint
        NavGotoWaypoint(WP_CLIMB);

        // Ramp up throttle
        NavVerticalAltitudeMode(300, 100);

    } else {

        // Log error that autopilot is already in flight mode
        fprintf(stderr, "WARNING: Autopilot is already in flight mode\n");

    }

}

struct POSITION getPosition(struct Navigator *self){

    // Initialize a position object
    struct POSITION currentPosition;

    // Sync the data with modules position
    currentPosition.x = stateGetPositionEnu_i()->x;
    currentPosition.y = stateGetPositionEnu_i()->y;
    currentPosition.alt = stateGetPositionEnu_i()->z;
    currentPosition.heading = stateGetNedToBodyEulers_f()->psi;

    // Return the position object
    return currentPosition;
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
    instance.getPosition = getPosition;
    instance.stop = stop;
    instance.takeoff = takeoff;
    instance.land = land;

    // Return the instance to the calling unit
    return instance;
}
