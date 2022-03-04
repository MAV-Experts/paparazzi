/*
 * Copyright (C) Group 10
 *
 * TU Delft, 04. Mar 2022
 */
/**
 * @file "modules/autonomous_mav/obstacle_detector/ObstacleDetector.h"
 * @author Group 10
 *
 */

#ifndef OBSTACLE_DETECTOR_H
#define OBSTACLE_DETECTOR_H

class ObstacleDetector {

public:

    /**
     * This method returns, whether there is an obstacle ahead of the aircraft
     *
     * @return obstacle in front of MAV
     */
    virtual bool obstacleAhead() = 0;

    /**
     * This method returns a 2D bool matrix which shows where the obstacle is located in front of the aircraft
     *
     * @return 2D obstacle matrix
     */
    virtual bool** getObstacleMap() = 0;

    /**
     * This method returns the distance of an obstacle ahead of the aircraft
     *
     * @return (closest) distance to obstacle
     */
    virtual double** getDistanceMap() = 0;

    /**
     * This method returns whether the obstacle detection unit is experiencing a fatal error
     *
     * @return ObstacleDetector is experiencing a fatal error
     */
    virtual bool hasError() = 0;

};

#endif

