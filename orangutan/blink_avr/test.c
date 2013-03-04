#include <avr/io.h> 
#include <util/delay.h>

/* 
 * Make DO output blink. TOGGLE flips the value, per OrangutanDigital.h 
 *
 * See /usr/lib/avr/include/avr/iom1284p.h for available functions
 */   
int main()  
{
    //IO_D0 = 0 
    unsigned char pin = 0;

    while(1) {
        //setup pin registers
        volatile unsigned char* pinRegister = (unsigned char*)&PIND;
        volatile unsigned char* portRegister = (unsigned char*)&PORTD;
        volatile unsigned char* ddrRegister = (unsigned char*)&DDRD;
        unsigned char bitmask = 1 << pin;

        //set output (toggle)
        *(portRegister) ^= bitmask;
        
        //set output direction  - 1 is output
        *(ddrRegister) |= bitmask;

        //from util/delay.h
        delay_ms(500);
    }  
}
