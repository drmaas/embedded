#include <pololu/orangutan.h>  
  
/* 
 * Make DO output blink. TOGGLE flips the value, per OrangutanDigital.h 
 */   
  
int main()  
{
    //IO_D0 = 0 
    unsigned char pin = 0;

    while(1)  
    {
        struct IOStruct registers;
        struct IOStruct* io = &registers;

        //from get_io_registers
        io->pinRegister = (unsigned char*)&PIND;
        io->portRegister = (unsigned char*)&PORTD;
        io->ddrRegister = (unsigned char*)&DDRD;
        io->bitmask = 1 << pin;

        //from set_digital_output_value - here we toggle it
        *(io->portRegister) ^= io->bitmask;
        
        //from set_data_direction - 1 is output
        *(io->ddrRegister) |= io->bitmask;

        delay_ms(500);
    }  
}
