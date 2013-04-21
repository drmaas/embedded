#include "timer.h"
#include "digital.h"
#include "interpolater.h"
#include "encoder.h"
#include "motor.h"

#include <avr/interrupt.h>

// GLOBALS
extern uint32_t f_IO;
extern uint32_t PID_ticks;
extern uint32_t INT_ticks;
extern int G_velocity_period;

volatile long curr_velocity = 0;
volatile long ref_pos = 0;
volatile int motor_mode = 0;

int timer_length;
char timer_tempBuffer[64];

void init_timers() {

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

        // SET appropriate bits in TCCR2A for fast PWM mode. This is mode 7 for fast pwm.
        TCCR2A |= (1 << WGM21);
        TCCR2A |= (1 << WGM20);

        // clear 0C2B on compare match
        TCCR2A |= (1 << COM2B1);

        // SET appropriate bits in TCCR2B for fast PWM mode. This is mode 7 for past pwm.
        TCCR2B |= (1 << WGM22);

        // Using pre-scaler 256, using CS2(2:0) = 6
        TCCR2B |= (1 << CS22);
        TCCR2B |= (1 << CS21);
        TCCR2B &= ~(1 << CS20);

        //TOP register is OCR2A, value is 100
        prescalar = 256;
        frequency = 781; //timer2 period to get us to TOP of 100
        OCR2A = get_timer_top_value(f_IO, prescalar, frequency);

        //clear OCR2B
        set_motor2_speed(0);

        //set direction to forward and clear PWM2B
        init_digital();

        //enable output compare match interrupt on timer 2
        TIMSK2 |= ( 1 << OCIE2A );
        TIMSK2 |= ( 1 << OCIE2B );

        // ----------------------------- TIMER1 setup for velocity calculation ---------------------//
        // Using CTC mode with OCR1A for TOP. This is mode 4, thus WGM3/3210 = 0100.
        TCCR1A &= ~(1 << WGM10);
        TCCR1A &= ~(1 << WGM11);
        TCCR1B |= (1 << WGM12);
        TCCR1B &= ~(1 << WGM13);

        // Toggle OC1A on a compare match. Thus COM1A_10 = 01
        TCCR1A &= ~(1 << COM1A1);
        TCCR1A |= (1 << COM1A0);

        // Using pre-scaler 1024. This is CS1/2/1/0 = 101
        TCCR1B |= (1 << CS12);
        TCCR1B &= ~(1 << CS11);
        TCCR1B |= (1 << CS10);

        // Interrupt Frequency: Hz? = f_IO / (1024*OCR1A)
        // Set OCR1A appropriately for TOP to generate desired frequency.
        // NOTE: This IS the toggle frequency.
        prescalar = 1024;
        frequency = 20.00;
        OCR1A = get_timer_top_value(f_IO, prescalar, frequency);

        // Regardless of its value (provided it is less than OCR1A), it will match at the frequency of timer 1.
        OCR1B = 1;

        //Enable output compare match interrupt on timer 1B
        TIMSK1 |= (1 << OCIE1A);

}

//get value we should set top register to based on clock speed, prescalar, and desired frequency
long get_timer_top_value(long clock, long prescalar, long frequency) {
    return clock/(prescalar*frequency);
}

//toggle between ref mode and constant mode
void set_mode(int mode) {
  motor_mode = mode;
}

//TIMER0 Interrupt for PID at 1KHz
//PID equation is T = Kp(Pr - Pm) - Kd*Vm
ISR(TIMER0_COMPA_vect) {
    //calculate current position
    long position = current_position();
    if (motor_mode == 1) {
        //calculate current speed 20x per second
        //if (PID_ticks % G_velocity_period == 0) {
        //    curr_velocity = calculate_velocity_ticks(position); //calculate_velocity(position,rate_sec);
        //}

        //set torque
        long torque = calculate_torque(get_kp(), get_kd(), ref_pos, position, curr_velocity);
        set_motor2_speed(torque);
    }

    // Increment ticks
    //timer_length = sprintf( timer_tempBuffer, "PID_ticks:%li\r\n", PID_ticks);
    //print_usb( timer_tempBuffer, timer_length );
    PID_ticks++;
}

//TIMER3 Interrupt for Interpolater at 500Hz
//PID equation is T = Kp(Pr - Pm) - Kd*Vm
ISR(TIMER3_COMPA_vect) {

        //calculate the reference position Pr to feed into PID equation
        long position = current_position();
        ref_pos = interpolate(position);

        //timer_length = sprintf( timer_tempBuffer, "Current:%li Reference: %li\r\n",position, ref_pos);
        //print_usb( timer_tempBuffer, timer_length );
 
        // Increment ticks
        //timer_length = sprintf( timer_tempBuffer, "INT_ticks:%li\r\n", INT_ticks);
        //print_usb( timer_tempBuffer, timer_length );
        INT_ticks++;
}

// INTERRUPT HANDLER for velocity calculation, 20x/sec
ISR(TIMER1_COMPA_vect) {
        long position = current_position();
        curr_velocity = calculate_velocity(position); //calculate_velocity(position,rate_sec);
}

//TIMER2 Interrupt for motor
ISR(TIMER2_COMPA_vect) {
}

ISR(TIMER2_COMPB_vect) {
}
