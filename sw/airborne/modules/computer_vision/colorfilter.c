/*
 * Copyright (C) 2015
 *
 * This file is part of Paparazzi.
 *
 * Paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * Paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * @file modules/computer_vision/colorfilter.c
 */

// Own header
#include "modules/computer_vision/colorfilter.h"
#include <stdio.h>

#include "modules/computer_vision/lib/vision/image.h"

#ifndef COLORFILTER_FPS
#define COLORFILTER_FPS 0       ///< Default FPS (zero means run at camera fps)
#endif
PRINT_CONFIG_VAR(COLORFILTER_FPS)


#ifndef COLORFILTER_SEND_OBSTACLE
#define COLORFILTER_SEND_OBSTACLE FALSE    ///< Default sonar/agl to use in opticflow visual_estimator
#endif
PRINT_CONFIG_VAR(COLORFILTER_SEND_OBSTACLE)

struct video_listener *listener = NULL;

// Filter Settings
uint8_t color_lum_min1 = 105;
uint8_t color_lum_max1 = 205;
uint8_t color_cb_min1  = 52;
uint8_t color_cb_max1  = 140;
uint8_t color_cr_min1  = 180;
uint8_t color_cr_max1  = 255;

// Result
volatile int color_count_orange = 0;

//Filter for white

uint8_t color_lum_min2 = 230;
uint8_t color_lum_max2 = 255;
uint8_t color_cb_min2  = 0;
uint8_t color_cb_max2  = 0;
uint8_t color_cr_min2  = 0;
uint8_t color_cr_max2  = 0;

// Result
volatile int color_count_white = 0;

#include "modules/core/abi.h"

// Function
static struct image_t *colorfilter_func(struct image_t *img, uint8_t camera_id __attribute__((unused)))
{
  // Filter
  color_count_orange = image_yuv422_colorfilt(img, img,
                                       color_lum_min, color_lum_max,
                                       color_cb_min, color_cb_max,
                                       color_cr_min, color_cr_max
                                      );
color_count_white = image_yuv422_colorfilt(img, img,
                                       color_lum_min, color_lum_max,
                                       color_cb_min, color_cb_max,
                                       color_cr_min, color_cr_max
                                      );

  if (COLORFILTER_SEND_OBSTACLE) {
    if (color_count_orange > 20 || color_count_white > 20)
    {
      if (color_count_orange > color_count_white){
        color_count_orange = color_count
      }
      else
      {
        color_count_white = color_count
      }
      AbiSendMsgOBSTACLE_DETECTION(OBS_DETECTION_COLOR_ID, 1.f, 0.f, 0.f);
    }
    else
    {
      AbiSendMsgOBSTACLE_DETECTION(OBS_DETECTION_COLOR_ID, 10.f, 0.f, 0.f);
    }
  }

  return img; // Colorfilter did not make a new image
}

void colorfilter_init(void)
{
  cv_add_to_device(&COLORFILTER_CAMERA, colorfilter_func, COLORFILTER_FPS, 0);
}
