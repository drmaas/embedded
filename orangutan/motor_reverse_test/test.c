#include <pololu/orangutan.h>

/*
 * Press button to start. Run motor 2 rotations one direction, 2 rotations other while button is not pressed. 
 * Next button press stops motor. Repeat.
 */

int main() {

  enum { FORWARD, REVERSE, STOP } m2_direction; 

  //motor speed with be either 128 or -128
  int motorSpeed = -128;

  while(1) {

    unsigned char button = get_single_debounced_button_press(ANY_BUTTON);

    //middle button pressed
    if (button & MIDDLE_BUTTON) {
   
      //set speed to 0 until release 
      set_m2_speed(0);
      wait_for_button_release(button & MIDDLE_BUTTON);    

      //now print to lcd and revert motor speed
      motorSpeed = -motorSpeed;
      lcd_goto_xy(0, 0);
      print("spd=");
      print_long(motorSpeed);        // print the resulting motor speed (-255 - 255)
      print("   ");
      set_m2_speed(motorSpeed);

      // all LEDs off
      red_led(0);
      green_led(0);
    
      // turn green LED on when motors are spinning forward
      if (motorSpeed > 0)
        green_led(1);
      // turn red LED on when motors are spinning in reverse
      if (motorSpeed < 0)
        red_led(1);
      delay_ms(100);

    }
  }
}
