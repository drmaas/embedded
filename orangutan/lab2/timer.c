#include "timer.h"
#include "digital.h"
#include "encoder.h"

#include <avr/interrupt.h>

// GLOBALS
extern uint32_t f_IO;
extern uint32_t T_ms_ticks;
extern int G_velocity_period;

volatile long period = 1000; //timer2 period
volatile long current_velocity = 0;

volatile int LOGGING = 0;

void init_timers() {

        int length;
        char tempBuffer[128];

        // -------------------------  TIMER2 setup --------------------------------------//
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
        long prescalar = 256;
        long frequency = period;
        OCR2A = get_timer_top_value(f_IO, prescalar, frequency);

        //clear OCR2B
        OCR2B = 0;

        //set direction to forward and clear PWM2B
        init_digital();

        //Enable output compare match interrupt on timer 2
        TIMSK2 |= ( 1 << OCIE2A );

        set_motor2_speed(20);

}

//get value we should set top register to based on clock speed, prescalar, and desired frequency
long get_timer_top_value(long clock, long prescalar, long frequency) {
    return clock/(prescalar*frequency);
}

//toggle logging
void toggle_logging() {
    LOGGING = ~LOGGING;
}

long get_current_velocity() {
    return current_velocity;
}

//Interrupt for COMPA on TIMER2
ISR(TIMER2_COMPA_vect) {

        // This is the Interrupt Service Routine for Timer2
        // Each time the TCNT count is equal to the OCR2 register, this interrupt is "fired".

        int length;
        char tempBuffer[64];

        //calculate current position
        long position = current_position();
     
        //calculate current speed
        if (T_ms_ticks % G_velocity_period == 0) {
            current_velocity = calculate_velocity(position);
        }
 
        length = sprintf( tempBuffer, "Timer2 position: %li velocity: %li.\r\n",position,current_velocity);
        //print_usb( tempBuffer, length );

        // Increment ticks
        T_ms_ticks++;

}
