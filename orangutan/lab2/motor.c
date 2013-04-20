#include "motor.h"
#include "digital.h"

int motor_length;
char motor_tempBuffer[128];

//assumes timer related to motor2 has already been initialized
//set speed to speed
void set_motor2_speed(long speed) {

        unsigned char reverse = 0;

        //check speed and set reverse if necessary
        if (speed < 0) {
                speed = -speed;
                reverse = 1;
        }
        if (speed > 255) {
                 speed = 255;
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

//calculate torque with PID equation T = Kp(Pr - Pm) - Kd*Vm
long calculate_torque(double kp, double kd, long pr, long pm, long vm) {
    long torque = (kp*(pr-pm) - kd*vm);

    //motor_length = sprintf( motor_tempBuffer, "pr:%li pm:%li vm:%li torque:%li\r\n",pr,pm,vm,torque);
    //print_usb( motor_tempBuffer, motor_length );

    return torque;
}

//return speed (T) from OCR2B
long get_motor2_speed() {
    return OCR2B;
}
