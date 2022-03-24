//
// Created by theo on 24-03-22.
//

#include "../Navigator.h"

#ifndef PAPARAZZI_BASICNAVIGATOR_H
#define PAPARAZZI_BASICNAVIGATOR_H

enum NAV_STATES getState(struct Navigator *self);
void computePath(struct Navigator *self, int obstacleMap);
int hasError(struct Navigator *self);
void start(struct Navigator *self);
void stop(struct Navigator *self);
void land(struct Navigator *self);
void takeoff(struct Navigator *self);

struct Navigator createBasicNavigator();

#endif //PAPARAZZI_BASICNAVIGATOR_H
