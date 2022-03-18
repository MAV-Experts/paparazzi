/*
 * Copyright (C) Group 10
 *
 * TU Delft, 04. Mar 2022
 */
/**
 * @file "modules/autonomous_mav/navigator/Navigator.h"
 * @author Group 10
 *
 */

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

// settings
extern float oag_color_count_frac;  // obstacle detection threshold as a fraction of total of image
extern float oag_floor_count_frac;  // floor detection threshold as a fraction of total of image
extern float oag_max_speed;         // max flight speed [m/s]
extern float oag_heading_rate;      // heading rate setpoint [rad/s]

extern void obstacle_avoider_guided_init(void);
extern void obstacle_avoider_guided_periodic(void);

#endif
