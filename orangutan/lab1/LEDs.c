#include "LEDs.h"
#include "timer.h"

#include <avr/io.h>
#include <stdio.h>

// RED LED is toggled in a cyclic executive defined in lab1.c
// Yellow and Green are toggled in interrupt handlers defined below.
// The interrupts are initialized in the timers.c file.
// Yellow is on Timer 3. Green is on Timer 1.

// GLOBALS
extern uint32_t G_yellow_ticks;

extern uint16_t G_red_period;
extern uint16_t G_green_period;
extern uint16_t G_yellow_period;

extern uint32_t G_red_toggles;
extern uint32_t G_green_toggles;
extern uint32_t G_yellow_toggles;

extern uint32_t f_IO;

char tempBuffer[64];

//setup values used by leds and perform startup test
void init_LEDs() {

        uint32_t i;

        // Clear all data direction ports
        DD_REG_RED = 0;
        DD_REG_YELLOW = 0;
        DD_REG_GREEN = 0;

        // Configure data direction as output
        DD_REG_RED |= BIT_RED;
        DD_REG_YELLOW |= BIT_YELLOW;
        DD_REG_GREEN |= BIT_GREEN;

        // Turn LEDs for two seconds to make sure they are working
        LED_ON(RED);
        LED_ON(YELLOW);

        //wait 1 sec 
        for (i=0;i<2000;i++) WAIT_1MS;

        // Start all LEDs off
        LED_OFF(RED);
        LED_OFF(YELLOW);

        // clear toggle counters
        G_green_toggles = 0;
        G_red_toggles = 0;
        G_yellow_toggles = 0;

}

void set_toggle(char color, int ms) {

		// check toggle ms is positive and multiple of 100
		if (ms<0) {
			return;
		}

		if (~((ms%100)==0)) {
			ms = ms - (ms%100);
		}
		
		// For each color, if ms is 0, turn it off by changing data direction to input.
		// If it is >0, set data direction to output.
		if ((color=='R') || (color=='A')) {
			if (ms==0) {
                            DD_REG_RED &= ~BIT_RED;
                        }
			else {
                            DD_REG_RED |= BIT_RED;
                        }
			G_red_period = ms;
		}
		if ((color=='Y') || (color=='A')) {
			if (ms==0) {
                            DD_REG_YELLOW &= ~BIT_YELLOW;
                        }
			else {
                            DD_REG_YELLOW |= BIT_YELLOW;
                        }
			G_yellow_period = ms;
		}
		if ((color=='G') || (color=='A')) {
			if (ms==0) {
                            DD_REG_GREEN &= ~BIT_GREEN;
                        }
			else {
                            DD_REG_GREEN |= BIT_GREEN;
                        }
			// green has a limit on its period.
			if ( ms > 4000) {
                            ms = 4000;
                        }
			G_green_period = ms;
			// set the OCR1A (TOP) to get (approximately) the requested frequency.
			if ( ms > 0 ) {
                            double frequency = 1000/ms;
                            long prescalar = 1024;
                            OCR1A = get_timer_top_value(f_IO, prescalar, frequency);
			}
 		}
}

// INTERRUPT Names are defined in iom1284p.h
// INTERRUPT HANDLER for yellow LED
ISR(TIMER3_COMPA_vect) {

	// This the Interrupt Service Routine for Toggling the yellow LED.
	// Each time the TCNT count is equal to the OCRxx register, this interrupt is enabled.
	// At creation of this file, it was initialized to interrupt every 100ms (10Hz).

	// Increment ticks. If it is time, toggle YELLOW and increment toggle counter.
        G_yellow_ticks++;
        
        if ((G_yellow_ticks*100 % G_yellow_period) == 0) {
            //int length = sprintf( tempBuffer, "Y toggles %d\r\n", G_yellow_toggles );
            //print_usb( tempBuffer, length );
#ifdef ECHO2LCD
            //lcd_goto_xy(0, 1);
            //printf("Y:%d", G_yellow_toggles);
#endif
            LED_TOGGLE(YELLOW);
            G_yellow_toggles++;
        }
}

// INTERRUPT HANDLER for green LED
ISR(TIMER1_COMPA_vect) {

	// This the Interrupt Service Routine for tracking green toggles. The toggling is done in hardware.
	// Each time the TCNT count is equal to the OCRxx register, this interrupt is enabled.
	// This interrupts at the user-specified frequency for the green LED.
	
        //int length = sprintf( tempBuffer, "G toggles %d\r\n", G_green_toggles );
        //print_usb( tempBuffer, length );
	G_green_toggles++;
}
