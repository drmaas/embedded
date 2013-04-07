#ifndef __encoder_H
#define __encoder_H

#include <pololu/orangutan.h>
#include <avr/io.h>         //gives us names for registers
#include <avr/interrupt.h>

#include <inttypes.h> //gives us uintX_t

#include <string.h>

#define WHEEL_TICKS 64
#define CIRCUMFERENCE 8 //about 8 inches

void init_encoder();
long current_position();
long calculate_velocity(long current_position);
unsigned char get_m2a_value();
unsigned char get_m2b_value();

#endif
