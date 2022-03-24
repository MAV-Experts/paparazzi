//
// Created by theo on 24-03-22.
//

#include "BasicDetector.h"

void init(struct ObstacleDetector *self){

    // Print message about startup of detector
    fprintf(stderr, "Initializing the basic obstacle detector ...\n");

    // Register image feed listeners
    cv_add_to_device(&DETECTOR_FRONT_CAMERA, storeFrontImage, DETECTOR_FPS, 0);
    cv_add_to_device(&DETECTOR_BOTTOM_CAMERA, storeBottomImage, DETECTOR_FPS, 0);

}

struct image_t * storeFrontImage(struct image_t *img, uint8_t filter){
    frontImageBuffer = *img;
    return img;
}

struct image_t * storeBottomImage(struct image_t *img, uint8_t filter){
    bottomImageBuffer = *img;
    return img;
}

int obstacleAhead(struct ObstacleDetector *self){

    // TODO: compute whether there is an obstacle ahead

    fprintf(stderr, "Front image is %d x %d\n", frontImageBuffer.w, frontImageBuffer.h);
    fprintf(stderr, "Bottom image is %d x %d\n", bottomImageBuffer.w, bottomImageBuffer.h);

    return 0;
}

int getObstacleMap(struct ObstacleDetector *self){

    // TODO: implement

    return 0;
}

int hasDetectionError(struct ObstacleDetector *self){

    // TODO: implement

    return 0;
}

int outOfArena(struct ObstacleDetector *self){

    // TODO: implement

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

//    instance.currentBottomImage = bottomImageBuffer;
//    instance.currentFrontImage = frontImageBuffer;

    // Return the final instance
    return instance;
}