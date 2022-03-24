//
// Created by theo on 24-03-22.
//

#include "../ObstacleDetector.h"

#ifndef PAPARAZZI_BASICDETECTOR_H
#define PAPARAZZI_BASICDETECTOR_H

static struct image_t *storeFrontImage(struct image_t *img, uint8_t filter);
static struct image_t *storeBottomImage(struct image_t *img, uint8_t filter);
void init(struct ObstacleDetector *self);
int obstacleAhead(struct ObstacleDetector *self);
int getObstacleMap(struct ObstacleDetector *self);
int hasDetectionError(struct ObstacleDetector *self);
int outOfArena(struct ObstacleDetector *self);

struct ObstacleDetector createBasicDetector();

#endif //PAPARAZZI_BASICDETECTOR_H
