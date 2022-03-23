/*
 * Copyright (C) 2019 Kirk Scheper <kirkscheper@gmail.com>
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
 * @file modules/computer_vision/cv_detect_object.h
 * Assumes the object consists of a continuous color and checks
 * if you are over the defined object or not
 */

// Own header
#include "modules/computer_vision/cv_detect_color_object.h"
#include "modules/computer_vision/cv.h"
#include "modules/core/abi.h"
#include "std.h"
#include "math.h"


#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "pthread.h"

//Optionally, include open cv here
#include "cv.h"

#define PRINT(string,...) fprintf(stderr, "[object_detector->%s()] " string,__FUNCTION__ , ##__VA_ARGS__)
#if OBJECT_DETECTOR_VERBOSE
#define VERBOSE_PRINT PRINT
#else
#define VERBOSE_PRINT(...)
#endif

static pthread_mutex_t mutex;

#ifndef COLOR_OBJECT_DETECTOR_FPS1
#define COLOR_OBJECT_DETECTOR_FPS1 0 ///< Default FPS (zero means run at camera fps)
#endif
#ifndef COLOR_OBJECT_DETECTOR_FPS2
#define COLOR_OBJECT_DETECTOR_FPS2 0 ///< Default FPS (zero means run at camera fps)
#endif

// Filter Settings
uint8_t cod_lum_min1 = 0;
uint8_t cod_lum_max1 = 0;
uint8_t cod_cb_min1 = 0;
uint8_t cod_cb_max1 = 0;
uint8_t cod_cr_min1 = 0;
uint8_t cod_cr_max1 = 0;

uint8_t cod_lum_min2 = 0;
uint8_t cod_lum_max2 = 0;
uint8_t cod_cb_min2 = 0;
uint8_t cod_cb_max2 = 0;
uint8_t cod_cr_min2 = 0;
uint8_t cod_cr_max2 = 0;

bool cod_draw1 = false;
bool cod_draw2 = false;

// define global variables
struct color_object_t {
  int32_t x_c;
  int32_t y_c;
  uint32_t color_count;
  bool updated;
};

struct color_object_t global_filters[2];

// struct that holds the data for the navigation function
// for some reason it does not compile when declared in the header :(
struct object_counts_t {
   uint32_t orange;
   uint32_t white_count;
   uint32_t zone1;
   uint32_t zone2;
   uint32_t zone3;
};

// Function, changed the function declaration to accomodate the struct
struct object_counts_t find_object_centroid(struct image_t *img, int32_t* p_xc, int32_t* p_yc, bool draw,
                              uint8_t lum_min, uint8_t lum_max,
                              uint8_t cb_min, uint8_t cb_max,
                              uint8_t cr_min, uint8_t cr_max);

// a function that creates filter presets for the find object centroid function,
// it just returns the img unchanged. 
static struct image_t *object_detector(struct image_t *img, uint8_t filter)
{
  uint8_t lum_min, lum_max;
  uint8_t cb_min, cb_max;
  uint8_t cr_min, cr_max;
  bool draw;

  // filter can be in two stages
  switch (filter){
    case 1:
      lum_min = cod_lum_min1;
      lum_max = cod_lum_max1;
      cb_min = cod_cb_min1;
      cb_max = cod_cb_max1;
      cr_min = cod_cr_min1;
      cr_max = cod_cr_max1;
      draw = cod_draw1;
      break;
    case 2:
      lum_min = cod_lum_min2;
      lum_max = cod_lum_max2;
      cb_min = cod_cb_min2;
      cb_max = cod_cb_max2;
      cr_min = cod_cr_min2;
      cr_max = cod_cr_max2;
      draw = cod_draw2;
      break;
    default:
      return img;
  };

  int32_t x_c, y_c;

  // Filter and find centroid, changed count to a pointer so it expects the array.
  struct object_counts_t count; // create the right return type for object centroid. 
  count = find_object_centroid(img, &x_c, &y_c, draw, lum_min, lum_max, cb_min, cb_max, cr_min, cr_max);

  // Print object count en treshold, print image centre
  VERBOSE_PRINT("Color count %d: %u, threshold %u, x_c %d, y_c %d\n", camera, object_count, count_threshold, x_c, y_c);
  VERBOSE_PRINT("centroid %d: (%d, %d) r: %4.2f a: %4.2f\n", camera, x_c, y_c,
        hypotf(x_c, y_c) / hypotf(img->w * 0.5, img->h * 0.5), RadOfDeg(atan2f(y_c, x_c)));

  //lock the mutual exclusion
  pthread_mutex_lock(&mutex);
  //color count
  global_filters[filter-1].color_count = count.orange; // store the orange value of the struct 
  global_filters[filter-1].x_c = x_c;
  global_filters[filter-1].y_c = y_c;
  //updated
  global_filters[filter-1].updated = true;
  //unlock the mutual exclusion
  pthread_mutex_unlock(&mutex);

  return img;
}

//Create an object detector with filter options 1
struct image_t *object_detector1(struct image_t *img, uint8_t camera_id);
struct image_t *object_detector1(struct image_t *img, uint8_t camera_id __attribute__((unused)))
{
  return object_detector(img, 1);
}

//Create an object detector with filter options 2
struct image_t *object_detector2(struct image_t *img, uint8_t camera_id);
struct image_t *object_detector2(struct image_t *img, uint8_t camera_id __attribute__((unused)))
{
  return object_detector(img, 2);
}

//initialization function
void color_object_detector_init(void)
{
  memset(global_filters, 0, 2*sizeof(struct color_object_t));
  pthread_mutex_init(&mutex, NULL);
  //Initiate first object detector mode
#ifdef COLOR_OBJECT_DETECTOR_CAMERA1
#ifdef COLOR_OBJECT_DETECTOR_LUM_MIN1
  cod_lum_min1 = COLOR_OBJECT_DETECTOR_LUM_MIN1;
  cod_lum_max1 = COLOR_OBJECT_DETECTOR_LUM_MAX1;
  cod_cb_min1 = COLOR_OBJECT_DETECTOR_CB_MIN1;
  cod_cb_max1 = COLOR_OBJECT_DETECTOR_CB_MAX1;
  cod_cr_min1 = COLOR_OBJECT_DETECTOR_CR_MIN1;
  cod_cr_max1 = COLOR_OBJECT_DETECTOR_CR_MAX1;
#endif
#ifdef COLOR_OBJECT_DETECTOR_DRAW1
  cod_draw1 = COLOR_OBJECT_DETECTOR_DRAW1;
#endif

  cv_add_to_device(&COLOR_OBJECT_DETECTOR_CAMERA1, object_detector1, COLOR_OBJECT_DETECTOR_FPS1, 0);
#endif

  //Initiate second object detector mode
#ifdef COLOR_OBJECT_DETECTOR_CAMERA2
#ifdef COLOR_OBJECT_DETECTOR_LUM_MIN2
  cod_lum_min2 = COLOR_OBJECT_DETECTOR_LUM_MIN2;
  cod_lum_max2 = COLOR_OBJECT_DETECTOR_LUM_MAX2;
  cod_cb_min2 = COLOR_OBJECT_DETECTOR_CB_MIN2;
  cod_cb_max2 = COLOR_OBJECT_DETECTOR_CB_MAX2;
  cod_cr_min2 = COLOR_OBJECT_DETECTOR_CR_MIN2;
  cod_cr_max2 = COLOR_OBJECT_DETECTOR_CR_MAX2;
#endif
#ifdef COLOR_OBJECT_DETECTOR_DRAW2
  cod_draw2 = COLOR_OBJECT_DETECTOR_DRAW2;
#endif

  cv_add_to_device(&COLOR_OBJECT_DETECTOR_CAMERA2, object_detector2, COLOR_OBJECT_DETECTOR_FPS2, 1);
#endif
}

/*
 * find_object_centroid
 *
 * Finds the centroid of pixels in an image within filter bounds.
 * Also returns the amount of pixels that satisfy these filter bounds.
 *
 * @param img - input image to process formatted as YUV422.
 * @param p_xc - x coordinate of the centroid of color object
 * @param p_yc - y coordinate of the centroid of color object
 * @param lum_min - minimum y value for the filter in YCbCr colorspace
 * @param lum_max - maximum y value for the filter in YCbCr colorspace
 * @param cb_min - minimum cb value for the filter in YCbCr colorspace
 * @param cb_max - maximum cb value for the filter in YCbCr colorspace
 * @param cr_min - minimum cr value for the filter in YCbCr colorspace
 * @param cr_max - maximum cr value for the filter in YCbCr colorspace
 * @param draw - whether or not to draw on image
 * @return number of pixels of image within the filter bounds.
 */
//Function declaration of the finding the centre of the image - find orange pixels
struct object_counts_t find_object_centroid(struct image_t *img, int32_t* p_xc, int32_t* p_yc, bool draw,
                              uint8_t lum_min, uint8_t lum_max,
                              uint8_t cb_min, uint8_t cb_max,
                              uint8_t cr_min, uint8_t cr_max)
{
  uint32_t cnt = 0;
  uint32_t white_count = 0;
  uint32_t tot_x = 0;
  uint32_t tot_y = 0;
  uint32_t slice;
  uint32_t previous_slice;
  uint8_t *we = img->buf;
  uint8_t *buffer = img->buf;
  u_int32_t white_threshold = 220;
  uint8_t previous_Y = 0;
  uint32_t edge_length = 8;
  uint32_t yp_mean;
  uint32_t yp_previous_mean;
  int32_t dY = 0; 
  //Keep a memory list of the lumination values
  uint32_t yp_memory_list[img->w];
  uint8_t edge_threshhold = 5;
  struct object_counts_t counts;          // the array that stores the # of orange pixels and zone counts. 
  uint16_t bin_size = img->w / 3;
  // Go through all the pixels


  // move along the y axis
  for (uint16_t y = 0; y < img->h; y++) {

	//for each row the previous illumation  0;
	previous_Y = 0;
	dY = 0;
  //clean the memory list
  memset(yp_memory_list, 0, img->w);

	//move along the x axis
    for (uint16_t x = 0; x < img->w; x ++) {
      slice = 0;
      previous_slice = 0;
      /*******Obtaining YUV colors**********/
      // Check if the color is inside the specified values
      uint8_t *yp, *up, *vp;
      int32_t yp_cache;

      if (x % 2 == 0) {
        // Even x
        up = &buffer[y * 2 * img->w + 2 * x];      // U
        yp = &buffer[y * 2 * img->w + 2 * x + 1];  // Y1
        vp = &buffer[y * 2 * img->w + 2 * x + 2];  // V
        //yp = &buffer[y * 2 * img->w + 2 * x + 3]; // Y2
      } else {
        // Uneven x
        up = &buffer[y * 2 * img->w + 2 * x - 2];  // U
        //yp = &buffer[y * 2 * img->w + 2 * x - 1]; // Y1
        vp = &buffer[y * 2 * img->w + 2 * x];      // V
        yp = &buffer[y * 2 * img->w + 2 * x + 1];  // Y2
      }

      /*********Detecting orange************/
      // Check between minimum and maximum values of the colors
      if ( (*yp >= lum_min) && (*yp <= lum_max) &&
           (*up >= cb_min ) && (*up <= cb_max ) &&
           (*vp >= cr_min ) && (*vp <= cr_max )) {
    	//Increase the pixel count
        counts.orange ++;
        //Add up aggregate of x and y values
        tot_x += x;
        tot_y += y;
      }

      // store the value of yp to use in the edge algorthm.
      yp_cache = *yp;  
      if (draw){
        *yp = 255;  // make pixel brighter in image
      }

      /*Jonathan Dijkstra - white detector using absolute illuminance pixel values */
      if(yp_cache >= white_threshold)
      {
        counts.white_count++;
      }


      /**Jonathan Dijkstra - edge detector using and difference in y components
       * 1. Calculate difference in lumanination (Y value) between two adjecent pixels
       * 2. If the difference is above some threshold, count an edge
       * 3. Keep track of the amount of edge detections in the x
       **/

      /* store current illumination value in the memory array */
      yp_memory_list[x] = yp_cache;

      //Start detecting edges only when enough data is available
      if((x + edge_length +1) > 0)
      {
        //collect all Y point in the edge length, and the previous ones
        for(uint32_t i = x - edge_length; i < x; i++)
        {
          slice += yp_memory_list[i];
          previous_slice += yp_memory_list[i-1];
        }
        //calculate the mean of the slices
        yp_mean = slice / edge_length;
        yp_previous_mean = previous_slice / edge_length;
      }

      //Calculate lumination differences in slices
      dY = abs(yp_mean - yp_previous_mean);
      //Check if the difference above a certain treshhold
      if(dY >= edge_threshhold)
      {
        // Find where the edge is
        if(x <= bin_size ) //x >= 0 && 
        {
          counts.zone1++;
        }
        else if(x > bin_size && x<= (2 * bin_size))
        {
          counts.zone2++;
        }
        else (x > (2 * bin_size) && x<= (3 * bin_size))
        {
          counts.zone3++;
        }
      }
      previous_Y = yp_cache;
    }
  }

  //Check if a color has been detected
  if (cnt > 0) {
	//Centroid of the x and y coordinates: divide the total by the count
    *p_xc = (int32_t)roundf(tot_x / ((float) cnt) - img->w * 0.5f);
    *p_yc = (int32_t)roundf(img->h * 0.5f - tot_y / ((float) cnt));
  } else {
	//Zero output in case no colored pixel has been detected
    *p_xc = 0;
    *p_yc = 0;
  }

  //Return the datatype containing all the information
  return counts;
}

//Periodic function of the object detector
void color_object_detector_periodic(void)
{
  static struct color_object_t local_filters[2];
  //Mutual exclusion with the navigator part. This file runs at 60Hz whereas the orange_avoider.c file runs at 500Hz.
  pthread_mutex_lock(&mutex);
  //Copy memory
  memcpy(local_filters, global_filters, 2*sizeof(struct color_object_t));
  //Unlock the mutual exclusion
  pthread_mutex_unlock(&mutex);

  /******Orange count*******/
  //If the filter has been updated (computation is done)
  if(local_filters[0].updated){
	//Send message with color count
    AbiSendMsgVISUAL_DETECTION(COLOR_OBJECT_DETECTION1_ID, local_filters[0].x_c, local_filters[0].y_c,
        0, 0, local_filters[0].color_count, 0);
    local_filters[0].updated = false;
  }

  /******Floor count*******/
  //If the filter has been updated (computation is done)
  if(local_filters[1].updated){
	  // send message with color count
    AbiSendMsgVISUAL_DETECTION(COLOR_OBJECT_DETECTION2_ID, local_filters[1].x_c, local_filters[1].y_c,
        0, 0, local_filters[1].color_count, 1);
    local_filters[1].updated = false;
  }

  /*******Edge count*******/
  
  if(local_filters[1].updated){
	  // send message with color count
    AbiSendMsgVISUAL_EdgeDETECTION(EDGE_OBJECT_DETECTION3_ID, 
    count.zone1, count.zone2, count.zone3, count.zone4, count.zone5);
    local_filters[2].updated = false;
  }
  
}