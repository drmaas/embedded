#include "interpolater.h"

volatile int position_degrees = 0;
volatile int KP = 0;
volatile int KD = 0;
volatile long user_start_pos = 0;
volatile long user_desired_pos = 0;

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
void set_kp(int kp) {
    KP = kp;
}

//get kp
int get_kp() {
    return KP;
}

//set kd
void set_kd(int kd) {
    KD = kd;
}

//get kd
int get_kd() {
    return KD;
}

//get torque(motor value) via equation T = Kp(Pr-Pm)-Kd*Vm
int interpolate(int vm, int pm) {

    return 0;
}
