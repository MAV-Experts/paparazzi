/*
 * Copyright (C) Group 10
 *
 * TU Delft, 04. Mar 2022
 */
/**
 * @file "modules/autonomous_mav/runner.h"
 * @author Group 10
 *
 * This module represents the brain of a MAV that flies fully autonomous through
 */

#include "AutonomousMAV.h"

/**
 * Paparazzi initialization function
 */
extern void runner_init(void);

/**
 * Paparazzi periodically called function
 */
extern void runner_periodic(void);