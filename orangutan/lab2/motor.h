#ifndef __motor_H
#define __motor_H

#include <avr/io.h>         //gives us names for registers
#include <avr/interrupt.h>

#include <inttypes.h> //gives us uintX_t

#include <string.h>

void set_motor2_speed(int speed);
int get_motor2_speed();

#endif
