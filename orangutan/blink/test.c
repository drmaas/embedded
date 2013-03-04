#include <pololu/orangutan.h>  
  
/* 
 * Make DO output blink. TOGGLE flips the value, per OrangutanDigital.h 
 */   
  
int main()  
{  
    while(1)  
    {  
        set_digital_output(IO_D0, TOGGLE);
        delay_ms(500);
    }  
}
