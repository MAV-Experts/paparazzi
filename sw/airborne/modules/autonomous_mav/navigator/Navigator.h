//
// Created by theo on 24-03-22.
//

#ifndef PAPARAZZI_NAVIGATOR_H
#define PAPARAZZI_NAVIGATOR_H

/**
 * The states that the Navigator can be in
 */
enum NAV_STATES {
    NAV_NORMAL,
    NAV_EVASIVE
};

/**
 * Instance of a Navigator
 *
 * @notes The navigator performs state-based computation of path and where to fly etc. by implementing the
 * different functions in the specific file.
 */
struct Navigator {

    /**
     * This attribute stores the current state of the Navigator
     */
    enum NAV_STATES currentState;

    /**
     * Method returns the current state of the navigation unit
     *
     * @param self reference to object
     * @return current navigation unit state
     */
    enum NAV_STATES (*getState)(struct Navigator *self);

    /**
     * This method is being called in the major FSM logic with the current obstacle map of
     *
     * @param self reference to object
     * @param obstacleMap 2D map normalized world map ahead
     */
    void (*computePath)(struct Navigator *self, int obstacleMap);

    /**
     * Method returns true when the navigator is experiencing any problems
     *
     * @param self reference to object
     * @return (1) if the navigator has problems
     */
    int (*hasError)(struct Navigator *self);

};

#endif //PAPARAZZI_NAVIGATOR_H
