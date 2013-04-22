#include "interpolater.h"
#include "encoder.h"

volatile int position_degrees = 0;

//initial KP: 50 = KP*16 = 3.125
//final KP: 16
volatile double KP = 12.0;
volatile double KD = 4.0;
volatile long user_start_pos = 0;
volatile long user_desired_pos = 0;

volatile long step_size = 16; // 90 degrees in ticks

volatile long ref_position;

int int_length;
char int_tempBuffer[64];

//get desired user position
long desired_position() {
    return user_desired_pos;
}

//set desired user position
void set_desired_position(long position) {
    user_desired_pos = position;
}

//get start position when command pressed
long start_position() {
    return user_start_pos;
}

//set start position when command pressed
void set_start_position(long position) {
    user_start_pos = position;
}

//set kp
void set_kp(double kp) {
    KP += kp;
}

//get kp
double get_kp() {
    return KP;
}

//set kd
void set_kd(double kd) {
    KD += kd;
}

//get kd
double get_kd() {
    return KD;
}

//get reference position to feed to equation T = Kp(Pr-Pm)-Kd*Vm
long interpolate(long curr_pos) {
    //rcomplete = 0;
    //calculate the reference position Pr to feed into PID equation
    //long start_pos = start_position();
    long distance_travelled = curr_pos; // - start_pos;
    long d = desired_position();
    long destination = angleToSteps(d);
    long distance_remaining = destination - distance_travelled;

    //update ref_position if we are outside of step_size window
    if (destination > 0) {
        if (distance_remaining >= step_size) {
            ref_position = distance_travelled + step_size;
        }
        else {
            ref_position = destination;
            if (distance_remaining == 0) {
                //rcomplete = 1;
            } 
        }
    }
    else {
        if (-distance_remaining >= step_size) {
            ref_position = distance_travelled - step_size;
        }
        else {
            ref_position = destination;
            if (distance_remaining == 0) {
                //rcomplete = 1;
            } 
        }
    }

    //int_length = sprintf( int_tempBuffer, "Des:%li Curr:%li Dest:%li Ref: %li\r\n",d,curr_pos, destination, ref_position);
    //print_usb( int_tempBuffer, int_length );

    return ref_position;
}

long get_ref_position() {
    return ref_position;
}
