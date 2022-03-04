/*
 * Copyright (C) Group 10
 *
 * TU Delft, 04. Mar 2022
 */
/**
 * @file "modules/autonomous_mav/navigator/Navigator.h"
 * @author Group 10
 *
 */

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

enum NAV_STATE {
    NAV_TAKEOFF,
    NAV_LANDING,
    NAV_NORMAL,
    NAV_EVASIVE
};

class Navigator {

public:

    /**
     * The methods start and stop can be used for initial operations
     *
     */
    virtual void start() = 0;
    virtual void stop() = 0;

    /**
     * This method returns whether the MAV is in an evasive action
     *
     * @return MAV currently performing evasive action
     */
    virtual NAV_STATE getState() = 0;

    /**
     * This method is called periodically to compute the route
     *
     * @param obstacleMap 2D matrix containing booleans whether there is an obstacle or not
     * @param distanceMap 2D matrix containing distance measures / estimates to front
     *
     * TODO: might have to be excluded in separate module routine (will have to check)
     */
    virtual void computePath(bool** obstacleMap, double** distanceMap) = 0;

    /**
     * This method returns whether the navigator is experiencing a fatal error
     *
     * @return Navigator is experiencing a fatal error
     */
    virtual bool hasError() = 0;

    /**
     * This method performs take off maneuver
     */
    virtual void takeoff() = 0;

    /**
     * This method performs landing maneuver
     */
    virtual void land() = 0;

};

#endif

