//
// Created by theo on 24-03-22.
//

#ifndef PAPARAZZI_OBSTACLEDETECTOR_H
#define PAPARAZZI_OBSTACLEDETECTOR_H

/**
 * Row instance of an Obstacle Detector
 *
 * @note The detector is an instance that performs image recognition tasks
 */
struct ObstacleDetector {

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
    int (*obstacleAhead)(struct ObstacleDetector *self);

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
