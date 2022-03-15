#ifndef BASIC_DETECTOR_C
#define BASIC_DETECTOR_C

#include "../ObstacleDetector.h"

void init(struct ObstacleDetector *self){
    // TODO: imlpement cool stuff
}

int obstacleAhead(struct ObstacleDetector *self){
    // TODO: imlpement cool stuff
    return 0;
}

int getObstacleMap(struct ObstacleDetector *self){
    // TODO: imlpement cool stuff
    return 0;
}

double getDistanceMap(struct ObstacleDetector *self){
    // TODO: imlpement cool stuff
    return 0;
}

int hasDetectionError(struct ObstacleDetector *self){
    // TODO: imlpement cool stuff
    return 0;
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