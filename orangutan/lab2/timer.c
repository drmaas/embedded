#include "timer.h"
#include "digital.h"
#include "interpolater.h"
#include "encoder.h"

#include <avr/interrupt.h>

// GLOBALS
extern uint32_t f_IO;
extern uint32_t PID_ticks;
extern uint32_t INT_ticks;
extern int G_velocity_period;

volatile long curr_velocity = 0;
volatile long ref_pos = 0;
volatile long step_size = 16; // 90 degrees in ticks

volatile int LOGGING = 0;

void init_timers() {

        int length;
        char tempBuffer[128];

        // -------------------------  TIMER0 setup for PID --------------------------------------//
        // Software Clock Using Timer/Counter 0.
        // THE ISR for this is below.

        // SET appropriate bits in TCCR for CTC mode. This is mode 2, thus WGM2 = 0, WGM0/1/0 = 2.
        TCCR0A |= (1 << WGM01);
        TCCR0B &= ~(1 << WGM02);

        //TOP register is OCR0A, value is 78
        //OCR0A = 78;
        long prescalar = 256;
        double frequency = 1000.00;
        OCR0A = get_timer_top_value(f_IO, prescalar, frequency);

        // Using pre-scaler 256, using CS0(2:0) = 4
        TCCR0B |= (1 << CS02);
        TCCR0B &= ~(1 << CS01);
        TCCR0B &= ~(1 << CS00);

        //Enable output compare match interrupt on timer 0
        TIMSK0 |= ( 1 << OCIE0A );

        //--------------------------- TIMER3 setup for interpolater ----------------------------------//
        // SET appropriate bits in TCCR ...
        // Using CTC mode with OCR3A for TOP. This is mode 4, thus WGM1/3210 = 0100.
        TCCR3A &= ~(1 << WGM30);
        TCCR3A &= ~(1 << WGM31);
        TCCR3B |= (1 << WGM32);
        TCCR3B &= ~(1 << WGM33);

        // Using pre-scaler 256, using CS0(2:0) = 4
        TCCR3B |= (1 << CS32);
        TCCR3B &= ~(1 << CS31);
        TCCR3B &= ~(1 << CS30);

        // Interrupt Frequency: 500 = f_IO / (prescaler*OCR3A)
        prescalar = 256;
        frequency = 500.00;
        OCR3A = get_timer_top_value(f_IO, prescalar, frequency);

        //Enable output compare match interrupt on timer 3A
        TIMSK3 |= ( 1 << OCIE3A );

        // -------------------------  TIMER2 setup for motor --------------------------------------//
        // Software Clock Using Timer/Counter 2.
        // THE ISR for this is below.

        // Software Clock Interrupt Frequency: 1000 = f_IO / (prescaler*OCR2A)
        //f_IO is 20MHz

        // SET appropriate bits in TCCR2A for fast PWM mode. This is mode 7.
        TCCR2A |= (1 << WGM21);
        TCCR2A |= (1 << WGM20);

        // clear 0C2A on compare match
        TCCR2A |= (1 << COM2B1);

        // SET appropriate bits in TCCR2B for fast PWM mode. This is mode 7.
        TCCR2B |= (1 << WGM22);

        // Using pre-scaler 256, using CS2(2:0) = 6
        TCCR2B |= (1 << CS22);
        TCCR2B |= (1 << CS21);
        TCCR2B &= ~(1 << CS20);

        //TOP register is OCR2A, value is 78
        prescalar = 256;
        frequency = 781; //timer2 period to get us to TOP of 100
        OCR2A = get_timer_top_value(f_IO, prescalar, frequency);

        //clear OCR2B
        OCR2B = 0;

        //set direction to forward and clear PWM2B
        init_digital();

        //Enable output compare match interrupt on timer 2
        TIMSK2 |= ( 1 << OCIE2A );

        //TEST
        set_motor2_speed(10);

}

//get value we should set top register to based on clock speed, prescalar, and desired frequency
long get_timer_top_value(long clock, long prescalar, long frequency) {
    return clock/(prescalar*frequency);
}

//toggle logging
void toggle_logging() {
    LOGGING = ~LOGGING;
}

//TIMER0 Interrupt for PID at 1KHz
//PID equation is T = Kp(Pr - Pm) - Kd*Vm
ISR(TIMER0_COMPA_vect) {

        int length;
        char tempBuffer[64];

        //calculate current position
        long position = current_position();

        //calculate current speed
        if (PID_ticks % G_velocity_period == 0) {
            curr_velocity = calculate_velocity(position);
        }

        length = sprintf( tempBuffer, "Motor position: %li velocity: %li.\r\n",position,curr_velocity);
        //print_usb( tempBuffer, length );

        // Increment ticks
        length = sprintf( tempBuffer, "PID_ticks:%li\r\n", PID_ticks);
        //print_usb( tempBuffer, length );
        PID_ticks++;
}

//TIMER3 Interrupt for Interpolater at 500Hz
//PID equation is T = Kp(Pr - Pm) - Kd*Vm
ISR(TIMER3_COMPA_vect) {

        int length;
        char tempBuffer[64];

        //calculate the reference position Pr to feed into PID equation
        long curr_pos = current_position(); //absolute
        long distance_travelled = curr_pos - start_position();
        long destination = angleToSteps(desired_position());
        long distance_remaining = destination - distance_travelled;

        //update ref_pos if we are outside of step_size window
        if (distance_remaining >= step_size) {
            ref_pos = distance_travelled + step_size; 
        }
 
        // Increment ticks
        length = sprintf( tempBuffer, "INT_ticks:%li\r\n", INT_ticks);
        //print_usb( tempBuffer, length );
        INT_ticks++;
}

//TIMER2 Interrupt for motor
ISR(TIMER2_COMPA_vect) {
}
