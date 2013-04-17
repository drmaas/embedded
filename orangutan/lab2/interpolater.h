#ifndef __interpolater_H
#define __interpolater_H

#include <avr/io.h>         //gives us names for registers
#include <avr/interrupt.h>

#include <inttypes.h> //gives us uintX_t

#include <string.h>

long desired_position();
void set_desired_position(long position);
long start_position();
void set_start_position(long position);
void set_kp(double kp);
double get_kp();
void set_kd(double kd);
double get_kd();

long interpolate(long curr_pos);

#endif
