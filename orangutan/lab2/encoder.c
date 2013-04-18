#include "encoder.h"

volatile unsigned char global_m2a;
volatile unsigned char global_m2b;
volatile long global_counts_m2;
volatile long global_error_m2;
volatile unsigned char global_last_m2a_val;
volatile unsigned char global_last_m2b_val;
volatile long prev_position = 0;
volatile long prev_velocity = 0;
volatile long start_pos = 0;
volatile long desired_pos = 0;
volatile long velocity = 0;

volatile long WHEEL_TICKS = 64;
volatile long CIRCLE = 360;
volatile long CIRCUMFERENCE = 8; //about 8 inches

extern int G_velocity_period;

int length;
char encoder_tempBuffer[64];

//initialize encoder
void init_encoder() {

        char m2a = PINA6;
        char m2b = PINA7;
 
        global_m2a = m2a;
        global_m2b = m2b;

        //set data direction as input
        DDRA &= ~(1 << m2a);
        DDRA &= ~(1 << m2b);

        //interrupts enabled on pins
        PCMSK0 |= (1 << m2a);
        PCMSK0 |= (1 << m2b);

        //initialize the global state
        global_counts_m2 = 0;
        global_error_m2 = 0;

        global_last_m2a_val = get_m2a_value();
        global_last_m2b_val = get_m2b_value();

        // For simplicity set all the bits in PCICR and let the enabling of
        // pin-change interrupts be solely controlled by PCMSKx bits.
        PCICR = 0xFF;

        // Clear the interrupt flags in case they were set before for any reason.
        // On the AVR, interrupt flags are cleared by writing a logical 1
        // to them.
        PCIFR = 0xFF;
}

//get digital input value m2a
unsigned char get_m2a_value() {
        return (PINA & (1 << global_m2a));
}

//get digital input value m2b
unsigned char get_m2b_value() {
        return (PINA & (1 << global_m2b));
}

//get current motor position in ticks
long current_position() {
    //cli(); 
    long counts = global_counts_m2;
    //sei();
    return counts;
}

//reset counts when target is met
long reset_counts() {
    global_counts_m2 = 0;
}

//convert angle to steps
long angleToSteps(long angle) {
    long steps = (angle*WHEEL_TICKS)/CIRCLE;
    length = sprintf( encoder_tempBuffer, "Angle: %li TICKS:%li C:%li steps:%li\r\n",angle,WHEEL_TICKS,CIRCLE,steps);
    //print_usb( encoder_tempBuffer, length );
    return steps; 
}

//convert to degrees by using 5.625 degrees/tick (360/64)
long current_degrees(long position) {
    return (position*(CIRCLE/WHEEL_TICKS))%360;
}

//get current velocity in rotations/min
long calculate_velocity(long position, double rate_sec) {
    //if (position == prev_position) {
    //    velocity = prev_velocity;
    //}
    //else {
        //velocity = (((position - prev_position)*(1000.00/G_velocity_period)*60)/WHEEL_TICKS)*CIRCUMFERENCE;
        velocity = (((position - prev_position)*rate_sec*60)/WHEEL_TICKS);
    //    prev_velocity = velocity;
    //}
    prev_position = position;
    return velocity;
}

//get current speed
long current_velocity() {
    return velocity;
}

//encoder interrupt
ISR(PCINT0_vect) {

        unsigned char m2a_val = get_m2a_value();
        unsigned char m2b_val = get_m2b_value();

        length = sprintf( encoder_tempBuffer, "Encoder test: %li,%li\r\n",global_counts_m2,global_error_m2);
        //print_usb( encoder_tempBuffer, length );

        char plus_m2 = m2a_val ^ global_last_m2b_val;
        char minus_m2 = m2b_val ^ global_last_m2a_val;

        if(plus_m2)
                global_counts_m2 += 1;
        if(minus_m2)
                global_counts_m2 -= 1;

        if(m2a_val != global_last_m2a_val && m2b_val != global_last_m2b_val)
                global_error_m2 = 1;

        global_last_m2a_val = m2a_val;
        global_last_m2b_val = m2b_val;

}
