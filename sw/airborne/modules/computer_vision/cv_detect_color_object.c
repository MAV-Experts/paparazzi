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
  uint32_t edge_count[3];
  bool updated;
};
struct color_object_t global_filters[2];

// struct that holds the data for the navigation function
// for some reason it does not compile when declared in the header :(
struct object_counts_t {
   uint32_t white_zone1;
   uint32_t white_zone2;
   uint32_t white_zone3;
   uint32_t orange_zone1;
   uint32_t orange_zone2;
   uint32_t orange_zone3;
   uint32_t edge_zone1;
   uint32_t edge_zone2;
   uint32_t edge_zone3;
   bool updated; 
};
// creating the variable where the counts are stored after the object detector function found them
struct object_counts_t global_zone_counts;

// Function, changed the function declaration to accomodate the struct
struct object_counts_t find_object_centroid(struct image_t *img, int32_t* p_xc, int32_t* p_yc, bool draw,
                              uint8_t lum_min, uint8_t lum_max,
                              uint8_t cb_min, uint8_t cb_max,
                              uint8_t cr_min, uint8_t cr_max);

// a function that creates filter presets for the find object centroid function,
// it just returns the img unchanged. 
// it also calls the find_object_centroid function and stores the output in the global variables 
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

  // Filter and find centroid, declare a struct data type count to store values from the find_object_centroid function
  // stores the detected counts in the struct
  struct object_counts_t detected_counts;
  detected_counts = find_object_centroid(img, &x_c, &y_c, draw, lum_min, lum_max, cb_min, cb_max, cr_min, cr_max);

  // Print object count en treshold, print image centre
  VERBOSE_PRINT("Color count %d: %u, threshold %u, x_c %d, y_c %d\n", camera, object_count, count_threshold, x_c, y_c);
  VERBOSE_PRINT("centroid %d: (%d, %d) r: %4.2f a: %4.2f\n", camera, x_c, y_c,
        hypotf(x_c, y_c) / hypotf(img->w * 0.5, img->h * 0.5), RadOfDeg(atan2f(y_c, x_c)));

  // storing the results from find_object in the global variables. 
  //lock the mutual exclusion
  pthread_mutex_lock(&mutex);
  //color count
  global_filters[filter-1].color_count = detected_counts.orange_zone1; // store the orange value of the struct this is incorrect and is just here to make it compile
  global_filters[filter-1].x_c = x_c;
  global_filters[filter-1].y_c = y_c;
  //updated
  global_filters[filter-1].updated = true;
  
  // store the white counts in the right zones
  global_zone_counts.white_zone1 = detected_counts.white_zone1;
  global_zone_counts.white_zone2 = detected_counts.white_zone2;
  global_zone_counts.white_zone3 = detected_counts.white_zone3;
  // store the orange counts in the right zones
  global_zone_counts.orange_zone1 = detected_counts.orange_zone1;
  global_zone_counts.orange_zone1 = detected_counts.orange_zone2;
  global_zone_counts.orange_zone1 = detected_counts.orange_zone3;
  // store the edge counts in the right zones
  global_zone_counts.edge_zone1 = detected_counts.edge_zone1;
  global_zone_counts.edge_zone1 = detected_counts.edge_zone2;
  global_zone_counts.edge_zone1 = detected_counts.edge_zone3;
  // tell the periodic function that there is new information to send. 
  global_zone_counts.updated = true;
  //unlock the mutual exclusion
  pthread_mutex_unlock(&mutex);

  return img;
}

//Create an object detector with filter options 1
// it calls the function implemented above wiht filter option 1 filled in.
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
  // reserving memory for the global struct to store the data for the object detection
  memset(global_filters, 0, 2*sizeof(struct color_object_t));
  memset(&global_zone_counts, 0, sizeof(struct object_counts_t)); 

  pthread_mutex_init(&mutex, NULL); // do not know what it does. 
  
  // settings for the color detecting modes ignoring it for now.
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
  uint32_t tot_x = 0;
  uint32_t tot_y = 0;
  uint8_t *we = img->buf;
  uint8_t *buffer = img->buf;
  uint8_t previous_Y = 0;
  int32_t dY = 0; 
  uint8_t edge_threshhold = 0;
  struct object_counts_t counts;          // the array that stores the # of orange pixels and zone counts. 
  uint16_t bin_size = img->w / 3;     // # of bins/zones in the image set to 3. 
  // Go through all the pixels
  // move along the y axis
  for (uint16_t y = 0; y < img->h; y++) {
	//for each row the previous illumation  0;
	previous_Y = 0;
	dY = 0;
	//move along the x axis
    for (uint16_t x = 0; x < img->w; x ++) {

      /*******Color detection *********/
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

      // Check between minimum and maximum values of the colors
      if ( (*yp >= lum_min) && (*yp <= lum_max) &&
           (*up >= cb_min ) && (*up <= cb_max ) &&
           (*vp >= cr_min ) && (*vp <= cr_max )) 
           {
    	      //Increase the pixel count
            counts.orange_zone1++;
            //Add up aggregate of x and y values
            tot_x += x;
            tot_y += y;
           }
        
        yp_cache = *yp; // store the value of yp to use in the edge algorthm. 
        if (draw){
          *yp = 255;  // make pixel brighter in image
        }

        /**Jonathan Dijkstra - edge detector using and difference in y components
         * 1. Calculate difference in lumanination (Y value) between two adjecent pixels
         * 2. If the difference is above some threshold, count an edge
         * 3. Keep track of the amount of edge detections in the x
         *
         * **/

        //Calculate difference in pixel lumination
        dY = abs(yp_cache - previous_Y);
        //Check if the difference above a certain treshhold
       if(dY >= edge_threshhold)
        {
        	// Find where the edge is
    	   if(x <= bin_size ) //x >= 0 && 
    	   {
    		  counts.edge_zone1++;
    	   }
    	   else if(x > bin_size && x<= (2 * bin_size))
    	   {
    		   counts.edge_zone2++;
    	   }
    	   else if(x > (2 * bin_size) && x<= (3 * bin_size))
    	   {
    		   counts.edge_zone3++;
    	   }
         // block that does two extra bins. which are not used
         /*
    	   else if(x > (3 * bin_size) && x<= (4 * bin_size))
    	   {
    		   counts.zone4++;
    	   }
    	   else
    	   {
    		   counts.zone5++;
    	   }
         */
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

  //Return the pixel count
  return counts;
}


//Periodic function of the object detector
void color_object_detector_periodic(void)
{
  static struct color_object_t local_filters[2];
  struct object_counts_t local_zone_counts;

  //Mutual exclusion with the navigator part. This file runs at 60Hz whereas the orange_avoider.c file runs at 500Hz.
  pthread_mutex_lock(&mutex);
  //Copy memory to the local filter from the global filters.
  memcpy(local_filters, global_filters, 2*sizeof(struct color_object_t));
  memcpy(&local_zone_counts, &global_zone_counts, sizeof(struct object_counts_t));
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
  
  if(local_zone_counts.updated){
	  // send message with zone counts
    AbiSendMsgZONE_COUNTS(ZONE_COUNTS_ID, 
    local_zone_counts.white_zone1, local_zone_counts.white_zone2, local_zone_counts.white_zone3, 
    local_zone_counts.orange_zone1, local_zone_counts.orange_zone2, local_zone_counts.orange_zone3,
    local_zone_counts.edge_zone1, local_zone_counts.edge_zone2, local_zone_counts.edge_zone3);
    
    local_zone_counts.updated = false; // tell the programm that the message is send.
  }
  
}
