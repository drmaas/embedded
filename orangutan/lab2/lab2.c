// definitions of registers in iom1284p.h: /usr/lib/avr/include/avr/iom1284p.h
//#define ECHO2LCD

#include <pololu/orangutan.h>

#include "timer.h"
#include "encoder.h"
#include "interpolater.h"
#include "menu.h"

#include <inttypes.h> //gives us uintX_t

// useful stuff from libc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// GLOBALS
volatile uint32_t f_IO = 20000000;
volatile uint32_t PID_ticks = 0;
volatile uint32_t INT_ticks = 0;
volatile uint32_t MAIN_ticks = 0;
volatile int G_velocity_period = 50;
volatile int LOGGING = 0;

int main_length;
char main_tempBuffer[64];

int main(void) {

	int i;

	clear();	// clear the LCD

	// Initialization here.
	init_menu();	// this is initialization of serial comm through USB
	lcd_init_printf();	// required if we want to use printf() for LCD printing
	init_timers();
	init_encoder();

	//enable interrupts
	sei();

        //poll	
	while (1) {
                MAIN_ticks++;

                //log
                if (LOGGING && (MAIN_ticks % 1000) == 0) {
                    long p = current_position();
                    long v = calculate_velocity_ticks(p);
                    main_length = sprintf( main_tempBuffer, "Motor position:%li Velocity:%li\r\n",p,v);
                    print_usb( main_tempBuffer, main_length );
                }

                serial_check();
                check_for_new_bytes_received();
	} //end while loop

} //end main
