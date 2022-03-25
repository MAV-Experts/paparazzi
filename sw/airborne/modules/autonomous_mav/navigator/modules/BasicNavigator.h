//
// Created by theo on 24-03-22.
//

#include "../Navigator.h"

#include "../../../../autopilot.h"
#include "../../../../state.h"
#include "firmwares/rotorcraft/navigation.h"
#include "../../../../firmwares/rotorcraft/autopilot_guided.h"
#include "../../../../firmwares/rotorcraft/navigation.h"
//#include "../../../../firmwares/rotorcraft/guidance/guidance_h.h"
// #include "../../../../firmwares/rotorcraft/navigation.h"
// #include "../../../nav/waypoints.h"
#include "generated/airframe.h"
#include "generated/flight_plan.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef PAPARAZZI_BASICNAVIGATOR_H
#define PAPARAZZI_BASICNAVIGATOR_H

enum NAV_STATES getState(struct Navigator *self);
void computePath(struct Navigator *self, struct DATA_MATRIX obstacleMap);
int hasError(struct Navigator *self);
struct POSITION getPosition(struct Navigator *self);
void start(struct Navigator *self);
void stop(struct Navigator *self);
void land(struct Navigator *self);
void takeoff(struct Navigator *self);

struct Navigator createBasicNavigator();

#endif //PAPARAZZI_BASICNAVIGATOR_H
