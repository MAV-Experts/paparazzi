//
// Created by theo on 23-03-22.
//

#include "AutonomousMAV.h"
#include "obstacle_detector/modules/BasicDetector.h"
#include "navigator/modules/BasicNavigator.h"

#ifndef PAPARAZZI_AUTONOMOUS_MAV_H
#define PAPARAZZI_AUTONOMOUS_MAV_H

extern float oa_color_count_frac;

extern void autonomous_mav_init(void);
extern void autonomous_mav_periodic(void);

#endif //PAPARAZZI_AUTONOMOUS_MAV_H
