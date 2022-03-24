//
// Created by theo on 24-03-22.
//

#include "BasicDetector.h"


void init(struct ObstacleDetector *self){

}

int obstacleAhead(struct ObstacleDetector *self){

    // TODO: compute whether there is an obstacle ahead

    return 0;
}

int getObstacleMap(struct ObstacleDetector *self){
    return 0;
}

int hasDetectionError(struct ObstacleDetector *self){
    return 0;
}

int outOfArena(struct ObstacleDetector *self){
    return 0;
}

struct ObstacleDetector createBasicDetector(){
    // Create an instance to return to requesting block
    struct ObstacleDetector instance;

    // Append the function to the object
    instance.init = init;
    instance.obstacleAhead = obstacleAhead;
    instance.getObstacleMap = getObstacleMap;
    instance.hasDetectionError = hasDetectionError;

    return instance;
}