/*
 * Copyright (C) Roland Meertens
 *
 * This file is part of paparazzi
 *
 */
// jESSE: what does this do? why is there a name ORANGE_AVOIDER_H?
#ifndef ORANGE_AVOIDER_H
#define ORANGE_AVOIDER_H

// settings
extern float ng_color_count_frac;

// functions
extern void orange_avoider_init(void);
extern void orange_avoider_periodic(void);

#endif