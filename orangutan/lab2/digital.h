#ifndef __digital_H
#define __digital_H

#include <avr/io.h>         //gives us names for registers
#include <avr/interrupt.h>

#include <inttypes.h> //gives us uintX_t

#include <string.h>

// define the data direction registers
#define DD_REG_DIRECTION DDRC
#define DD_REG_PWM DDRD

// define the output ports by which you send signals to the LEDs
#define PORT_DIRECTION PORTC
#define PORT_PWM PORTD

// define the bit-masks for each port that the LEDs are attached to
#define BIT_DIRECTION      ( 1 << PORTC6 )
#define BIT_PWM    ( 1 << PORTD6 )

// define "function" calls for turning LEDs on and off
#define PIN_ON(x)     (PORT_##x |= BIT_##x)
#define PIN_OFF(x)    (PORT_##x &= ~(BIT_##x))
#define PIN_TOGGLE(x) (PORT_##x ^= BIT_##x)

// function call prototypes
void init_digital();

#endif
