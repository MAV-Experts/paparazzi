//
// Created by theo on 24-03-22.
//

#include "./BasicDetector.h"

void init(struct ObstacleDetector *self){

    // Print message about startup of detector
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicDetector: init()\n");

    // Register image feed listeners
    cv_add_to_device(&DETECTOR_FRONT_CAMERA, storeFrontImage, DETECTOR_FPS, 0);
    cv_add_to_device(&DETECTOR_BOTTOM_CAMERA, storeBottomImage, DETECTOR_FPS, 0);

    // Initialize the data matrix with zeros
    for(int row = 0; row < MATRIX_ROWS; row++){
        for(int col = 0; col < MATRIX_COLS; col++){
            self->datamatrix.data[row][col].orange = 0;
            self->datamatrix.data[row][col].blue = 0;
            self->datamatrix.data[row][col].green = 0;
            self->datamatrix.data[row][col].obstacle_probability = 0;
        }
    }

}

struct image_t * storeFrontImage(struct image_t *img, uint8_t filter){
    frontImageBuffer = *img;
    return img;
}

struct image_t * storeBottomImage(struct image_t *img, uint8_t filter){
    bottomImageBuffer = *img;
    return img;
}

bool obstacleAhead(struct ObstacleDetector *self){

    // Log status update of navigator
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicDetector: obstacleAhead()\n");

    // TODO: compute whether there is an obstacle ahead

    return false;
}

struct DATA_MATRIX getObstacleMap(struct ObstacleDetector *self){

    // Log status update of navigator
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicDetector: getObstacleMap()\n");

    // TODO: some data matrix modifications here

    return self->datamatrix;
}

bool hasDetectionError(struct ObstacleDetector *self){

    // Log status update of navigator
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicDetector: hasDetectionError()\n");

    // TODO: implement

    return false;
}

bool outOfArena(struct ObstacleDetector *self){

    // Log status update of navigator
    if(LOG_UNIT_METHOD_CALLS)
        fprintf(stderr, "BasicDetector: outOfArena()\n");

    // TODO: implement

    return false;
}

struct ObstacleDetector createBasicDetector(){

    // Create an instance to return to requesting block
    struct ObstacleDetector instance;

    // Append the function to the object
    instance.init = init;
    instance.obstacleAhead = obstacleAhead;
    instance.getObstacleMap = getObstacleMap;
    instance.hasDetectionError = hasDetectionError;
    instance.outOfArena = outOfArena;

    // TODO: invent great way of appending images here
//    instance.currentBottomImage = bottomImageBuffer;
//    instance.currentFrontImage = frontImageBuffer;

    // Return the final instance
    return instance;
}