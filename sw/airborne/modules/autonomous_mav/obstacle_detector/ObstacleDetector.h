//
// Created by theo on 24-03-22.
//

#include "firmwares/rotorcraft/navigation.h"
#include "generated/airframe.h"
#include "./../../computer_vision/cv.h"
#include "./../../computer_vision/lib/vision/image.h"
#include "./../../core/abi.h"
#include "state.h"
#include "std.h"
#include "pthread.h"
// #include "./../../computer_vision/cv_detect_color_object.h"

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#ifndef PAPARAZZI_OBSTACLEDETECTOR_H
#define PAPARAZZI_OBSTACLEDETECTOR_H

/**
 * Row instance of an Obstacle Detector
 *
 * @note The detector is an instance that performs image recognition tasks
 */
struct ObstacleDetector {

    /**
     * This attribute stores the current image of the front camera
     */
//    image_t currentFrontImage;

    /**
     * This attribute stores the current image of the bottom camera
     */
//    image_t currentBottomImage;

    /**
     * This method contains initialization stuff for the specific detector unit
     *
     * @param self reference to object
     */
    void (*init)(struct ObstacleDetector *self);

    /**
     * TODO: write name and so on
     *
     * @param img
     * @param filter
     * @return
     */
//    struct image_t (*storeFrontImage)(struct image_t *img, uint8_t filter);
//    struct image_t (*storeBottomImage)(struct image_t *img, uint8_t filter);

    /**
     * This method returns true, when there i an obstacle ahead of the aircraft
     *
     * @param self reference to object
     * @return whether there is an obstacle in front
     */
    int (*obstacleAhead)(struct ObstacleDetector *self);

    /**
     * This method returns true if the aircraft is out of bounds of the cyber zoo
     *
     * @param self reference to object
     * @return (1) if out of arena
     */
    int (*outOfArena)(struct ObstacleDetector *self);

    /**
     * This method returns a 2D matrix of ints where encoded there is distance estimates
     *
     * @notes normalized values to depict world
     * @param self reference to object
     * @return 2D map of world ahead
     */
    int (*getObstacleMap)(struct ObstacleDetector *self);

    /**
     * This method returns whether the obstacle detection unit is experiencing a fatal error
     *
     * @param self reference to object
     * @return (1) if unit is experiencing an error
     */
    int (*hasDetectionError)(struct ObstacleDetector *self);

};

#endif //PAPARAZZI_OBSTACLEDETECTOR_H
