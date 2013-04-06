// definitions of registers in iom1284p.h: /usr/lib/avr/include/avr/iom1284p.h
#define ECHO2LCD

#include <pololu/orangutan.h>

#include "timer.h"
#include "menu.h"

#include <inttypes.h> //gives us uintX_t

// useful stuff from libc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// GLOBALS
volatile uint32_t f_IO = 20000000;
volatile uint32_t T_ms_ticks = 0;

int main(void) {

	int i;

	// Used to print to serial comm window
	char tempBuffer[32];
	int length = 0;
	
	clear();	// clear the LCD

	// Ininitialization here.
	lcd_init_printf();	// required if we want to use printf() for LCD printing
	init_timers();
	init_menu();	// this is initialization of serial comm through USB

	//enable interrupts
	sei();
	
	while (1) {
                serial_check();
                check_for_new_bytes_received();
	} //end while loop
} //end main
