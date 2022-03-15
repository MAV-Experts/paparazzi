#ifndef OBSTACLE_DETECTOR
#define OBSTACLE_DETECTOR

/**
* This file contains the description of the Obstacle Detector instances
 *
 * @note In the modules directory, different implementations of the ObstacleDetector are given
 * @date 08.03.2022
*/

/**
 * Raw instance of a Navigator
 *
 * @note The detector is an instance that performs image recognition tasks
 */
struct ObstacleDetector {

    /**
     * This method contains initialization stuff for the specific detector unit
     */
    void (*init)(struct ObstacleDetector *self);

    /**
     * This method returns, whether there is an obstacle ahead of the aircraft
     *
     * @return BOOL obstacle in front of MAV
     */
    int (*obstacleAhead)(struct ObstacleDetector *self);

    /**
     * This method returns a 2D bool matrix which shows where the obstacle is located in front of the aircraft
     *
     * TODO: This has to return a matrix of bool (check C style)
     *
     * @return 2D BOOL obstacle matrix
     */
    int (*getObstacleMap)(struct ObstacleDetector *self);

    /**
     * This method returns the distance of an obstacle ahead of the aircraft
     *
     * TODO: This has to return a matrix of double (check C style)
     *
     * @return (closest) distance to obstacle
     */
    double (*getDistanceMap)(struct ObstacleDetector *self);

    /**
     * This method returns whether the obstacle detection unit is experiencing a fatal error
     *
     * @return BOOL ObstacleDetector is experiencing a fatal error
     */
    int (*hasDetectionError)(struct ObstacleDetector *self);

};

#endif