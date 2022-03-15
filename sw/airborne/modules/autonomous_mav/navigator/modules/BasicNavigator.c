#include "BasicNavigator.h"


void start(struct Navigator *self){
    // TODO: imlpement cool stuff
}

void stop(struct Navigator *self){
    // TODO: imlpement cool stuff
}

enum NAV_STATE getState(struct Navigator *self){
    // TODO: imlpement cool stuff
    return NAV_NORMAL;
}

void computePath(struct Navigator *self, int obstacleMap, double distanceMap){
    // TODO: imlpement cool stuff
}

int hasError(struct Navigator *self){
    // TODO: imlpement cool stuff
    return 0;
}

void takeoff(struct Navigator *self){
    // TODO: imlpement cool stuff
}

void land(struct Navigator *self){
    // TODO: imlpement cool stuff
}

struct Navigator createBasicNavigator(){
    // Create an object of it
    struct Navigator instance;

    // Do initialization of object TODO: should be some startup state
    instance.currentState = NAV_NORMAL;

    // Append all methods to the struct
    instance.start = start;
    instance.stop = stop;
    instance.getState = getState;
    instance.computePath = computePath;
    instance.hasError = hasError;
    instance.takeoff = takeoff;
    instance.land = land;

    // Return the instance
    return instance;
}