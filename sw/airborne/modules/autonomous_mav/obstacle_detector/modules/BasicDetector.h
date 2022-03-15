//
// Created by Theo Rieken on 15.03.22.
//

#ifndef AUTONOMOUS_MAV_BASIC_DETECTOR_H
#define AUTONOMOUS_MAV_BASIC_DETECTOR_H

#include "../ObstacleDetector.h"

void init(struct ObstacleDetector *self);
int obstacleAhead(struct ObstacleDetector *self);
int getObstacleMap(struct ObstacleDetector *self);
double getDistanceMap(struct ObstacleDetector *self);
int hasDetectionError(struct ObstacleDetector *self);

struct ObstacleDetector createBasicDetector();


#endif //AUTONOMOUS_MAV_BASIC_DETECTOR_H
