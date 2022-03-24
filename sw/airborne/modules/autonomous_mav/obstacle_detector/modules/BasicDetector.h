//
// Created by theo on 24-03-22.
//

#include "../ObstacleDetector.h"
#include "modules/orange_avoider/orange_avoider.h"
#include "firmwares/rotorcraft/navigation.h"
#include "generated/airframe.h"
#include "state.h"
#include "modules/core/abi.h"
#include <time.h>
#include <stdio.h>

#ifndef PAPARAZZI_BASICDETECTOR_H
#define PAPARAZZI_BASICDETECTOR_H

void init(struct ObstacleDetector *self);
int obstacleAhead(struct ObstacleDetector *self);
int getObstacleMap(struct ObstacleDetector *self);
int hasDetectionError(struct ObstacleDetector *self);
int outOfArena(struct ObstacleDetector *self);

struct ObstacleDetector createBasicDetector();

#endif //PAPARAZZI_BASICDETECTOR_H
