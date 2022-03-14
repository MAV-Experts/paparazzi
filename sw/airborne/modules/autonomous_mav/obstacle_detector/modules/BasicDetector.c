#ifndef BASIC_DETECTOR_C
#define BASIC_DETECTOR_C

#include "../ObstacleDetector.h"
#include "stdbool.h"

void init(struct ObstacleDetector *self){
    // TODO: imlpement cool stuff
}

bool obstacleAhead(struct ObstacleDetector *self){
    // TODO: imlpement cool stuff
    return false;
}

bool getObstacleMap(struct ObstacleDetector *self){
    // TODO: imlpement cool stuff
    return false;
}

double getDistanceMap(struct ObstacleDetector *self){
    // TODO: imlpement cool stuff
    return 0;
}

bool hasDetectionError(struct ObstacleDetector *self){
    // TODO: imlpement cool stuff
    return false;
}

struct ObstacleDetector createBasicDetector(){
    // Create the raw instance
    struct ObstacleDetector instance;

    // Do initialization of object
    // TODO: "constructor" stuff goes here

    // Append all function to the struct
    instance.init = init;
    instance.obstacleAhead = obstacleAhead;
    instance.getObstacleMap = getObstacleMap;
    instance.getDistanceMap = getDistanceMap;
    instance.hasDetectionError = hasDetectionError;

    return instance;
}

#endif