#ifndef __encoder_H
#define __encoder_H

#include <pololu/orangutan.h>
#include <avr/io.h>         //gives us names for registers
#include <avr/interrupt.h>

#include <inttypes.h> //gives us uintX_t

#include <string.h>

void init_encoder();
long current_position();
long reset_counts();
long angleToSteps(long angle);
long current_degrees(long position);
long calculate_velocity_ticks(long position);
long current_velocity();
unsigned char get_m2a_value();
unsigned char get_m2b_value();

#endif
