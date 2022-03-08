#ifndef BASIC_DETECTOR_C
#define BASIC_DETECTOR_C

#include "../ObstacleDetector.h"
#include "stdbool.h"

void init(struct ObstacleDetector *self){

}

bool obstacleAhead(struct ObstacleDetector *self){
    return false;
}

bool getObstacleMap(struct ObstacleDetector *self){
    return false;
}

double getDistanceMap(struct ObstacleDetector *self){
    return 0;
}

bool hasDetectionError(struct ObstacleDetector *self){
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