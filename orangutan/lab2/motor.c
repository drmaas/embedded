#include "motor.h"
#include "digital.h"

//assumes timer related to motor2 has already been initialized
void set_motor2_speed(int speed) {

        unsigned char reverse = 0;

        //check speed and set reverse if necessary
        if (speed < 0) {
                speed = -speed;
                reverse = 1;
        }
        if (speed > 0xFF) {
                 speed = 0xFF;
        }

        //set speed
        OCR2B = speed;

        if (speed == 0) {
                //set 0% duty cycle on PWM pin by disconnecting it from timer2
                TCCR2A &= ~(1<<COM2B1);
        }
        else {
                //set variable duty cycle on PWM pin
                TCCR2A |= 1<<COM2B1;
  
                if (reverse) {
                       //backward
                       PIN_ON(DIRECTION);
                }
                else {
                       //forward
                       PIN_OFF(DIRECTION);
                }
        }

}
