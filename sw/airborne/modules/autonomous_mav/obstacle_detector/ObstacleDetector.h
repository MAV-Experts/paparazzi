//
// Created by theo on 24-03-22.
//

#ifndef PAPARAZZI_OBSTACLEDETECTOR_H
#define PAPARAZZI_OBSTACLEDETECTOR_H

#include "../AutonomousMAV.h"
#include "firmwares/rotorcraft/navigation.h"
#include "generated/airframe.h"
#include "./../../computer_vision/cv.h"
#include "./../../computer_vision/lib/vision/image.h"
#include "./../../core/abi.h"
#include "state.h"
#include "std.h"
#include "pthread.h"
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

/**
 * A datapoint is a single part of the data matrix
 */
struct DATA_POINT {

    // The amount of orange, green and blue
    uint16_t orange;
    uint16_t green;
    uint16_t blue;

    // The probability of an obstacle in this area
    uint16_t obstacle_probability;
};

/**
 * This stores the data matrix
 */
struct DATA_MATRIX {

    // data store
    struct DATA_POINT data[MATRIX_ROWS][MATRIX_COLS];

};

/**
 * Global image buffer for the front one
 *
 * @note the paparazzi task
 */
struct image_t frontImageBuffer;

/**
 * Global image buffer for the bottom one
 */
struct image_t bottomImageBuffer;


/**
 * Row instance of an Obstacle Detector
 *
 * @note The detector is an instance that performs image recognition tasks
 */
struct ObstacleDetector {

    /**
     * Declaration of the data matrix to be passed
     */
    struct DATA_MATRIX datamatrix;

    /**
     * This attribute stores the current image of the front camera
     */
    struct image_t currentFrontImage;

    /**
     * This attribute stores the current image of the bottom camera
     */
    struct image_t currentBottomImage;

    /**
     * This method contains initialization stuff for the specific detector unit
     *
     * @param self reference to object
     */
    void (*init)(struct ObstacleDetector *self);

    /**
     * This method returns true, when there i an obstacle ahead of the aircraft
     *
     * @param self reference to object
     * @return whether there is an obstacle in front
     */
    bool (*obstacleAhead)(struct ObstacleDetector *self);

    /**
     * This method returns true if the aircraft is out of bounds of the cyber zoo
     *
     * @param self reference to object
     * @return (1) if out of arena
     */
    bool (*outOfArena)(struct ObstacleDetector *self);

    /**
     * This method returns a 2D matrix of ints where encoded there is distance estimates
     *
     * @notes normalized values to depict world
     * @param self reference to object
     * @return 2D map of world ahead
     */
    struct DATA_MATRIX (*getObstacleMap)(struct ObstacleDetector *self);

    /**
     * This method returns whether the obstacle detection unit is experiencing a fatal error
     *
     * @param self reference to object
     * @return (1) if unit is experiencing an error
     */
    bool (*hasDetectionError)(struct ObstacleDetector *self);

};

#endif //PAPARAZZI_OBSTACLEDETECTOR_H
