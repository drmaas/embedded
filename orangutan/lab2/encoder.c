#include "encoder.h"

volatile unsigned char global_m2a;
volatile unsigned char global_m2b;
volatile long global_counts_m2;
volatile long global_error_m2;
volatile unsigned char global_last_m2a_val;
volatile unsigned char global_last_m2b_val;
volatile long prev_position = 0;
volatile long prev_time = 0;
volatile long prev_velocity = 0;

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
long current_angle_position() {
    return global_counts_m2;
}

//get current velocity in inches/min
long current_velocity(long current_position, long delay) {
    long velocity = 0;
    if (current_position == prev_position) {
        velocity = prev_velocity;
    }
    else {
        velocity = (((current_position - prev_position)*(1000.00/delay)*60)/WHEEL_TICKS)*CIRCUMFERENCE;
        prev_velocity = velocity;
    }
    prev_position = current_position;
    return velocity;
}

//encoder interrupt
ISR(PCINT0_vect) {

        int length;
        char tempBuffer[64];

        unsigned char m2a_val = get_m2a_value();
        unsigned char m2b_val = get_m2b_value();

        length = sprintf( tempBuffer, "Encoder test: %li,%li\r\n",global_counts_m2,global_error_m2);
        //print_usb( tempBuffer, length );

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
