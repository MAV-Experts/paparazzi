//
// Created by theo on 24-03-22.
//

#ifndef PAPARAZZI_BASICNAVIGATOR_H
#define PAPARAZZI_BASICNAVIGATOR_H

#include "../Navigator.h"

enum NAV_STATES getState(struct Navigator *self);
void computePath(struct Navigator *self, int obstacleMap);
int hasError(struct Navigator *self);

struct Navigator createBasicNavigator();

#endif //PAPARAZZI_BASICNAVIGATOR_H
