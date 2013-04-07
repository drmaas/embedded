#include "interpolater.h"

volatile int position_degrees = 0;
volatile int KP = 0;
volatile int KD = 0;

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

//set position (degrees from current)
void set_desired_position(int position) {
    position_degrees = position;
}

//get desired position
int get_desired_position() {
    return position_degrees;
}

//get torque(motor value) via equation T = Kp(Pr-Pm)-Kd*Vm
int interpolate(int vm, int pm) {

    return 0;
}
