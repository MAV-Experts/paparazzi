//
// Created by Theo Rieken on 15.03.22.
//

#ifndef AUTONOMOUS_MAV_BASIC_NAVIGATOR_H
#define AUTONOMOUS_MAV_BASIC_NAVIGATOR_H

#include "../Navigator.h"

void start(struct Navigator *self);
void stop(struct Navigator *self);
enum NAV_STATE getState(struct Navigator *self);
void computePath(struct Navigator *self, int obstacleMap, double distanceMap);
int hasError(struct Navigator *self);
void takeoff(struct Navigator *self);
void land(struct Navigator *self);

/**
 * Creator of the MAV instance
 *
 * @return Basic Navigator
 */
struct Navigator createBasicNavigator();

#endif //AUTONOMOUS_MAV_BASIC_NAVIGATOR_H
