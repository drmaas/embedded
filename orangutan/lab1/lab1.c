// definitions of registers in iom1284p.h:
// /usr/lib/avr/include/avr/iom1284p.h
#define ECHO2LCD
#define USE_RED_TIMER 1

#include <pololu/orangutan.h>

#include "LEDs.h"
#include "timer.h"
#include "menu.h"

//Gives us uintX_t (e.g. uint32_t - unsigned 32 bit int)
//On the ATMega128 int is actually 16 bits, so it is better to use
//  the int32_t or int16_t so you know exactly what is going on
#include <inttypes.h> //gives us uintX_t

// useful stuff from libc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// GLOBALS
volatile uint32_t G_yellow_ticks = 0;
volatile uint32_t G_ms_ticks = 0;

volatile uint16_t G_red_period = 1000;
volatile uint16_t G_green_period = 1000;
volatile uint16_t G_yellow_period = 1000;

volatile uint16_t G_release_red = 0;

volatile uint32_t G_red_toggles = 0;
volatile uint32_t G_green_toggles = 0;
volatile uint32_t G_yellow_toggles = 0;

volatile uint32_t f_IO = 20000000;

volatile uint8_t G_flag = 0; // generic flag for debugging

int main(void) {
	// -------------------------------------------------------------
	// This program teaches you about the various ways to "schedule" tasks.
	// You can think of the three blinking LEDs as separate tasks with 
	// strict timing constraints.
	//
	// As you build your program, think about the guarantees that you can 
	// make regarding the meeting of deadlines. Think about how the CPU
	// is "communicating" with the LEDs. Obviously, everything is output,
	// but what if it was input? Also think about how data can be passed
	// among tasks and how conflicts might arise.
	//
	// You will construct this program in pieces.
	// First, establish WCET analysis on a for loop to use for timing.
	// Use the for loop to blink the red LED.
        //	
        // Next, set up a system 1 ms software timer, and use that to "schedule" the blink
	// inside a cyclic executive.
	//
	// Blink the yellow LED using a separate timer with a 100ms resolution.
	// Blink the LED inside the ISR.
	//
	// Finally, blink the green LED by toggling the output on a pin using
	// a Compare Match. This is the creation of a PWM signal with a very long period.
	//	
	// --------------------------------------------------------------

	int i;

	// Used to print to serial comm window
	char tempBuffer[32];
	int length = 0;
	
	clear();	// clear the LCD

	// Ininitialization here.
	lcd_init_printf();	// required if we want to use printf() for LCD printing
	init_timers();
	init_LEDs();
	init_menu();	// this is initialization of serial comm through USB

	//enable interrupts
	sei();
	
	while (1) {
		//1) BEGIN with a simple toggle using for-loops. No interrupt timers */
		// toggle the LED. Increment a counter.

                if (USE_RED_TIMER != 1) {
		    //1) BEGIN with a simple toggle using for-loops. No interrupt timers */
		    // toggle the LED. Increment a counter.
                    LED_TOGGLE(RED);
		    G_red_toggles++;
		    //length = sprintf( tempBuffer, "R toggles %d\r\n", G_red_toggles );
		    //print_usb( tempBuffer, length );

		    // create a for-loop to kill approximately 1 second
		    for (i=0;i<1000;i++) {
		        WAIT_1MS;
		    }
                }
                else {
		    //2) use a software timer to "schedule" the RED LED toggle.
		    if (G_release_red) {

			G_release_red = 0; 

                        //print debug info
                        //length = sprintf( tempBuffer, "R toggles %d\r\n", G_red_toggles );
                        //print_usb( tempBuffer, length );
                        //toggle red led and increment count
			LED_TOGGLE(RED);
			G_red_toggles++;
                    }
		}

		// Whenever you are ready, add in the menu task.
		// Think of this as an external interrupt "releasing" the task.
		serial_check();
		check_for_new_bytes_received();
					
	} //end while loop
} //end main

