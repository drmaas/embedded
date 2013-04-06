#include "digital.h"
#include "timer.h"

#include <avr/io.h>
#include <stdio.h>

// GLOBALS
extern uint32_t f_IO;

char tempBuffer[64];

//setup values used by motor
void init_digital() {

        // Clear all data direction ports
        DD_REG_DIRECTION = 0;
        DD_REG_PWM = 0;

        // Configure data direction as output
        DD_REG_DIRECTION |= BIT_DIRECTION;
        DD_REG_PWM |= BIT_PWM;

        //motor direction set to forward (0) and pwm set to 0 (this was done in pololu src code)
        PIN_OFF(DIRECTION);
        PIN_OFF(PWM);
}
