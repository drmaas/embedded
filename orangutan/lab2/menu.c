#define ECHO2LCD

#include "menu.h"
#include "digital.h"
#include "motor.h"
#include "encoder.h"
#include "interpolater.h"
#include "timer.h"

#include <stdio.h>
#include <inttypes.h>

// GLOBALS

// local "global" data structures
unsigned char receive_buffer_position;
char receive_buffer[128];
char send_buffer[128];
char menuBuffer[128];
int received = 0;
char enter = 0x0D;

// Used to pass to USB_COMM for serial communication
int length;
char menu_tempBuffer[64];

extern int LOGGING;

// A generic function for whenever you want to print to your serial comm window.
// Provide a string and the length of that string. My serial comm likes "\r\n" at 
// the end of each string (be sure to include in length) for proper linefeed.
void print_usb( char *buffer, int n ) {
	serial_send( USB_COMM, buffer, n );
	wait_for_sending_to_finish();
}	
		
//------------------------------------------------------------------------------------------
// Initialize serial communication through USB and print menu options
// This immediately readies the board for serial comm
void init_menu() {
	
	// Set the baud rate to 9600 bits per second.  Each byte takes ten bit
	// times, so you can get at most 960 bytes per second at this speed.
	serial_set_baud_rate(USB_COMM, 9600);

	// Start receiving bytes in the ring buffer.
	serial_receive_ring(USB_COMM, receive_buffer, 32); //sizeof(receive_buffer));

	print_usb( "USB Serial Initialized\r\n", 24);
	print_usb( MENU, MENU_LENGTH );
}

//------------------------------------------------------------------------------------------
// process_received_string: Parses a menu command (series of keystrokes) that 
// has been received on USB_COMM and processes it accordingly.
// The menu command is buffered in check_for_new_bytes_received (which calls this function).
void process_received_string(const char* buffer)
{
	// parse and echo back to serial comm window (and optionally the LCD)
	char op_char;
	int value;
	int parsed;
	parsed = sscanf(buffer, "%c %d", &op_char, &value);
#ifdef ECHO2LCD
	lcd_goto_xy(0,0);
	printf("Got %c %d\n", op_char, value);
#endif
	length = sprintf( menu_tempBuffer, "Op:%c V:%d\r\n", op_char, value );
	print_usb( menu_tempBuffer, length );
	
	// Check valid command and implement
        long kd,kp,pu,pr,pm,vm,t;
        int mode = 0;
	switch (op_char) {
                // set desired speed for testing (this is T)
                case 'S':
                case 's':
                        mode = 0; 
                        set_mode(mode); //constant mode
                        set_motor2_speed(value);
                        break;
		// set desired positon (degrees from current)
		case 'R':
		case 'r':
                        mode = 1;
                        set_mode(mode); //ref mode
                        reset_counts(); //reset encoder count
                        set_desired_position(value);
			break; 
                // start logging pr, pm, T
                case 'L':
                case 'l':
                        LOGGING = ~LOGGING;
                        break;
		// print values of Kd, Kp, Vm, Pr, Pm, and T 
		case 'V':
		case 'v':
                        kd = (long)get_kd();
                        kp = (long)get_kp();
                        pu = desired_position();
                        pr = get_ref_position();
                        pm = current_position();
                        vm = calculate_velocity_ticks(pm);
                        t = get_motor2_speed();
			length = sprintf( menu_tempBuffer, "Current parameters: kp=%li kd=%li vm=%li pu=%li pr=%li pm=%li t=%li\r\n", kp,kd,vm,pu,pr,pm,t );
			print_usb( menu_tempBuffer, length ); 
			break;
                //increase kp
                case 'P':
                        set_kp(value);
                        break;
                //decrease kp
                case 'p':
                        set_kp(-value);
                        break;
                //increase kd
                case 'D':
                        set_kd(value);
                        break;
                //decrease kd
                case 'd':
                        set_kd(-value);
                        break;
		default:
			print_usb( "Command does not compute.\r\n", 27 );
		} // end switch(op_char) 
	
	print_usb( MENU, MENU_LENGTH);

} //end process_received_string()

//---------------------------------------------------------------------------------------
// If there are received bytes to process, this function loops through the receive_buffer
// accumulating new bytes (keystrokes) in another buffer for processing.
void check_for_new_bytes_received() {
	/* 
	The receive_buffer is a ring buffer. The call to serial_check() (you should call prior to this function) fills the buffer.
	serial_get_received_bytes is an array index that marks where in the buffer the most current received character resides. 
	receive_buffer_position is an array index that marks where in the buffer the most current PROCESSED character resides. 
	Both of these are incremented % (size-of-buffer) to move through the buffer, and once the end is reached, to start back at the beginning.
	This process and data structures are from the Pololu library. See examples/serial2/test.c and src/OrangutanSerial/*.*
	
	A carriage return from your comm window initiates the transfer of your keystrokes.
	All key strokes prior to the carriage return will be processed with a single call to this function (with multiple passes through this loop).
	On the next function call, the carriage return is processes with a single pass through the loop.
	The menuBuffer is used to hold all keystrokes prior to the carriage return. The "received" variable, which indexes menuBuffer, is reset to 0
	after each carriage return.
	*/ 
        int i = 0;
	
	// while there are unprocessed keystrokes in the receive_buffer, grab them and buffer
	// them into the menuBuffer
	while(serial_get_received_bytes(USB_COMM) != receive_buffer_position)
	{
                //break and process at 32 byte limit
                if (received >= 30) {
                    menuBuffer[received] = enter;
                    received++;
                    break;
                }

		// place in a buffer for processing
		menuBuffer[received] = receive_buffer[receive_buffer_position];
		received++;

		// Increment receive_buffer_position, but wrap around when it gets to
		// the end of the buffer. 
		if ( receive_buffer_position == sizeof(receive_buffer) - 1 )
		{
			receive_buffer_position = 0;
		}			
		else
		{
			receive_buffer_position++;
		}
	}

	// If there were keystrokes processed, check if a menu command
	if (received > 0 && (menuBuffer[received-1]==enter)) {
		
		if ( 1 == received ) {
	        	received = 0;
	        	return;
		}
		// Process buffer: terminate string, process, reset index to beginning of array to receive another command
		menuBuffer[received] = '\0';
#ifdef ECHO2LCD
		lcd_goto_xy(0,1);			
		print("RX: (");
		print_long(received-1);
		print_character(')');
                int i = 0;
		for (i=0; i<received-1; i++)
		{
	            print_character(menuBuffer[i]);
		}
#endif
		process_received_string(menuBuffer);
		received = 0;
	}
}
	
//-------------------------------------------------------------------------------------------
// wait_for_sending_to_finish:  Waits for the bytes in the send buffer to
// finish transmitting on USB_COMM.  We must call this before modifying
// send_buffer or trying to send more bytes, because otherwise we could
// corrupt an existing transmission.
void wait_for_sending_to_finish()
{
	while(!serial_send_buffer_empty(USB_COMM))
		serial_check();		// USB_COMM port is always in SERIAL_CHECK mode
}

