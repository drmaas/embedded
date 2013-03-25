/*******************************************
*
* Header file for Timer stuff.
*
*******************************************/
#ifndef __TIMER_H
#define __TIMER_H

#include <inttypes.h> //gives us uintX_t

// number of empty for loops to eat up about 1 ms
//#define FOR_COUNT_1MS 1082
#define FOR_COUNT_1MS 541

volatile uint32_t __ii;

#define WAIT_1MS {for (__ii=0;__ii<FOR_COUNT_1MS; __ii++);}

#define G_TIMER_RESOLUTION 100
#define Y_TIMER_RESOLUTION 100

void init_timers();

long get_timer_top_value(long clock, long prescalar, double frequency);

#endif //__TIMER_H
