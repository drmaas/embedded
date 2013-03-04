#include <pololu/orangutan.h>  
  
/* 
 * Make DO output blink. TOGGLE flips the value, per OrangutanDigital.h 
 */   
  
int main()  
{ 
    unsigned char pin = IO_D0;
    unsigned char outputState = 1;
    unsigned char bitmask = 1; 
    while(1)  
    {
        struct IOStruct registers;
        get_io_registers(&registers, pin);

//*(ioPin->portRegister) ^= ioPin->bitmask;  
        outputState ^= bitmask; 
        set_digital_output_value(&registers, outputState);
        set_data_direction(&registers, 1);

        delay_ms(500);
    }  
}
