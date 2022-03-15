/**
* This file contains the description of the Obstacle Detector instances
 *
 * @note In the modules directory, different implementations of the ObstacleDetector are given
 * @date 08.03.2022
*/

#ifndef AUTONOMOUS_MAV_NAVIGATOR_H
#define AUTONOMOUS_MAV_NAVIGATOR_H

/**
 * The states that the Navigator can be in
 */
enum NAV_STATE {
    NAV_TAKEOFF,
    NAV_LANDING,
    NAV_NORMAL,
    NAV_EVASIVE
};

/**
 * Raw instance of a Navigator
 *
 * @note The navigator performs state-based computation of path and where to fly etc. by implementing the different
 * functions in the specific c file.
 */
struct Navigator {

    /**
     * This attribute stores the current state value
     */
    enum NAV_STATE currentState;

    /**
     * The methods start and stop can be used for initial operations
     *
     */
    void (*start)(struct Navigator *self);
    void (*stop)(struct Navigator *self);

    /**
     * This method returns whether the MAV is in an evasive action
     *
     * @return MAV currently performing evasive action
     */
    enum NAV_STATE (*getState)(struct Navigator *self);

    /**
     * This method is called periodically to compute the route
     *
     * @param obstacleMap 2D matrix containing booleans whether there is an obstacle or not
     * @param distanceMap 2D matrix containing distance measures / estimates to front
     *
     * TODO: might have to be excluded in separate module routine (will have to check)
     */
    void (*computePath)(struct Navigator *self, int obstacleMap, double distanceMap);

    /**
     * This method returns whether the navigator is experiencing a fatal error
     *
     * @return Navigator is experiencing a fatal error
     */
    int (*hasError)(struct Navigator *self);

    /**
     * This method performs take off maneuver
     */
    void (*takeoff)(struct Navigator *self);

    /**
     * This method performs landing maneuver
     */
    void (*land)(struct Navigator *self);

};

#endif //AUTONOMOUS_MAV_NAVIGATOR_H