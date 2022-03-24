//
// Created by theo on 24-03-22.
//

#ifndef PAPARAZZI_BASICDETECTOR_H
#define PAPARAZZI_BASICDETECTOR_H

#include "../ObstacleDetector.h"

void init(struct ObstacleDetector *self);
int obstacleAhead(struct ObstacleDetector *self);
int getObstacleMap(struct ObstacleDetector *self);
int hasDetectionError(struct ObstacleDetector *self);
int outOfArena(struct ObstacleDetector *self);

struct ObstacleDetector createBasicDetector();

#endif //PAPARAZZI_BASICDETECTOR_H
