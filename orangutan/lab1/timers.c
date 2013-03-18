#include "timer.h"
#include "LEDs.h"

#include <avr/interrupt.h>

// GLOBALS
extern uint32_t G_green_ticks;
extern uint32_t G_yellow_ticks;
extern uint32_t G_ms_ticks;

extern uint16_t G_red_period;
extern uint16_t G_green_period;
extern uint16_t G_yellow_period;

extern uint16_t G_release_red;

void init_timers() {

        int length;
        char tempBuffer[64];

        // -------------------------  RED --------------------------------------//
        // Software Clock Using Timer/Counter 0.
        // THE ISR for this is below.

        // Software Clock Interrupt Frequency: 1000 = f_IO / (prescaler*OCR0A)
        //f_IO is 20MHz

        // SET appropriate bits in TCCR for CTC mode. This is mode 2, thus WGM2 = 0, WGM0/1/0 = 2.
        TCCR0A |= (1 << WGM01);
        TCCR0B &= ~(1 << WGM02);

        //TOP register is OCR0A, value is 78
        OCR0A = 78;

        // Using pre-scaler 256, using CS0(2:0) = 4
        TCCR0B |= (1 << CS02);
        TCCR0B &= ~(1 << CS01);
        TCCR0B &= ~(1 << CS00);

	length = sprintf( tempBuffer, "Initializing red clock to freq %dHz (period %d ms)\r\n",1000,1);	
        print_usb( tempBuffer, length );

        //Enable output compare match interrupt on timer 0
        TIMSK0 |= ( 1 << OCIE0A );

        // Initialize counter
        G_ms_ticks = 0;

	//--------------------------- YELLOW ----------------------------------//
	// Set-up of interrupt for toggling yellow LEDs. 
	// This task is "self-scheduled" in that it runs inside the ISR that is 
	// generated from a COMPARE MATCH of 
	// Timer/Counter 1 to OCR3A.
	// Obviously, we could use a single timer to schedule everything, but we are experimenting here!
	// THE ISR for this is in the LEDs.c file

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

	// Interrupt Frequency: 10 = f_IO / (prescaler*OCR3A)
        // 10 = 20M / (256*7812.5)
	// Set OCR3A appropriately for TOP to generate desired frequency using Y_TIMER_RESOLUTION (100 ms).
	// NOTE: This is not the toggle frequency, rather a tick frequency used to time toggles.
	OCR3A = 7812;

	length = sprintf( tempBuffer, "Initializing yellow clock to freq %dHz (period %d ms)\r\n",G_yellow_period,100);	
        print_usb( tempBuffer, length );

	//Enable output compare match interrupt on timer 3A
        TIMSK3 |= ( 1 << OCIE3A );

	G_yellow_ticks = 0;

	//--------------------------- GREEN ----------------------------------//
	// Set-up of interrupt for toggling green LED. 
	// This "task" is implemented in hardware, because the OC1A pin will be toggled when 
	// a COMPARE MATCH is made of 
	//      Timer/Counter 1 to OCR1A.
	// We will keep track of the number of matches (thus toggles) inside the ISR (in the LEDs.c file)
	// Limits are being placed on the frequency because the frequency of the clock
	// used to toggle the LED is limited.

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

	// Interrupt Frequency: ? = f_IO / (1024*OCR1A)
	// Set OCR1A appropriately for TOP to generate desired frequency.
	// NOTE: This IS the toggle frequency.
        // Test this with frequency 1, so OCR1A=19531
        OCR1A = 19531;
	length = sprintf( tempBuffer, "Initializing green clock to freq %dHz (period %d ms)\r\n",G_green_period,1);	
        print_usb( tempBuffer, length );

	// A match to this will toggle the green LED.
	// Regardless of its value (provided it is less than OCR3A), it will match at the frequency of timer 3.
	OCR1B = 1;

	//Enable output compare match interrupt on timer 1B
        TIMSK1 |= (1 << OCIE1A);
}

//INTERRUPT HANDLERS

//Red LED interrupt for COMPA on TIMER0
ISR(TIMER0_COMPA_vect) {

        // This is the Interrupt Service Routine for Timer0
        // Each time the TCNT count is equal to the OCR0 register, this interrupt is "fired".

        // Increment ticks
        G_ms_ticks++;

        // if time to toggle the RED LED, set flag to release
        if ( ( G_ms_ticks % G_red_period ) == 0 ) {
                G_release_red = 1;
        }
}
