//
// Created by theo on 24-03-22.
//

#include "BasicDetector.h"

struct image_t * storeFrontImage(struct image_t *img, uint8_t filter){
    fprintf(stderr, "Received front image ...\n");
    return img;
}

struct image_t * storeBottomImage(struct image_t *img, uint8_t filter){
    fprintf(stderr, "Received bottom image ...\n");
    return img;
}

void init(struct ObstacleDetector *self){
    fprintf(stderr, "Initializing the obstacle detector ...\n");

    // Register image feed listeners
    cv_add_to_device(&DETECTOR_FRONT_CAMERA, storeFrontImage, DETECTOR_FPS, 0);
    cv_add_to_device(&DETECTOR_BOTTOM_CAMERA, storeBottomImage, DETECTOR_FPS, 0);
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
//    instance.storeFrontImage = storeFrontImage;
//    instance.storeBottomImage = storeBottomImage;
    instance.obstacleAhead = obstacleAhead;
    instance.getObstacleMap = getObstacleMap;
    instance.hasDetectionError = hasDetectionError;

    return instance;
}