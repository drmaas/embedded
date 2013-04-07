// definitions of registers in iom1284p.h: /usr/lib/avr/include/avr/iom1284p.h
//#define ECHO2LCD

#include <pololu/orangutan.h>

#include "timer.h"
#include "encoder.h"
#include "menu.h"

#include <inttypes.h> //gives us uintX_t

// useful stuff from libc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// GLOBALS
volatile uint32_t f_IO = 20000000;
volatile uint32_t T_ms_ticks = 0;
volatile int G_velocity_period = 500;

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
                serial_check();
                check_for_new_bytes_received();
	} //end while loop

} //end main
