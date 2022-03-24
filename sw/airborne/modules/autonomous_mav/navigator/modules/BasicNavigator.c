//
// Created by theo on 24-03-22.
//

#include "BasicNavigator.h"

enum NAV_STATES getState(struct Navigator *self){
    return NAV_NORMAL;
}

void computePath(struct Navigator *self, int obstacleMap){
    // TODO: implement
}

int hasError(struct Navigator *self){

    return 0;
}

void start(struct Navigator *self){
    // TODO: implement
}

void stop(struct Navigator *self){
    // TODO: implement
}

void land(struct Navigator *self){
    // TODO: implement
}

void takeoff(struct Navigator *self){
    // TODO: implement
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

