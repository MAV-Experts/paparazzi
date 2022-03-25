//
// Created by theo on 24-03-22.
//

#ifndef PAPARAZZI_BASICDETECTOR_H
#define PAPARAZZI_BASICDETECTOR_H

#include "../ObstacleDetector.h"

// #include "../../../nav/waypoints.h"
#include "firmwares/rotorcraft/navigation.h"
#include "../../../computer_vision/video_thread_nps.h"

static struct image_t *storeFrontImage(struct image_t *img, uint8_t filter);
static struct image_t *storeBottomImage(struct image_t *img, uint8_t filter);

void init(struct ObstacleDetector *self);
bool obstacleAhead(struct ObstacleDetector *self);
struct DATA_MATRIX getObstacleMap(struct ObstacleDetector *self);
bool hasDetectionError(struct ObstacleDetector *self);
bool outOfArena(struct ObstacleDetector *self);

struct ObstacleDetector createBasicDetector();

#endif //PAPARAZZI_BASICDETECTOR_H
