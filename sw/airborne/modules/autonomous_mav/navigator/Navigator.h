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

class Navigator {

public:

    /**
     * This method returns whether the MAV is in an evasive action
     *
     * @return MAV currently performing evasive action
     */
    virtual bool evasiveActionInProgress() = 0;

    /**
     * This function is called in normal in flight MAV state to compute the next waypoint
     */
    virtual void computeNextWaypoint() = 0;

    /**
     * This method is called when there is an obstacle in the way to plan good escape route
     *
     * @param distance the closest distance to the obstacle
     * @param obstacleMap the map that contains all the
     */
    virtual void computeEvasiveWaypoint(double[] obstacleMap, double distance) = 0;

    /**
     * Thie method returns whether the navigator is experiencing a fatal error
     *
     * @return Navigator is experiencing a fatal error
     */
    virtual bool hasError() = 0;

};

#endif

