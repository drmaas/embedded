#include <pololu/orangutan.h>

/*
 * Press button to start. Run motor 2 rotations one direction, 2 rotations other while button is not pressed. 
 * Next button press stops motor. Repeat loop.
 *
 * Each wheel rotation is about 66 counts.
 *
 * Author: Dan Maas
 * 
 */

int main() {

  int MEDIUM_SPEED = 128;
  int TWO_TURNS = 132;

  enum STATE { ON, OFF } m2_state = OFF; 
  enum STATE next_m2_state = OFF; 

  //motor speed with be either 128 or -128
  int motorSpeed = 0;
  int last_motorSpeed = MEDIUM_SPEED;
  int pos = 0;

  //setup encoder ports
  encoders_init(-1, -1, IO_A6, IO_A7);

  while(1) {

    unsigned char button = get_single_debounced_button_press(ANY_BUTTON);

    //middle button pressed - turn motor on and off
    if (button & MIDDLE_BUTTON) {
  
      //wait until release 
      wait_for_button_release(button & MIDDLE_BUTTON);    

      switch (m2_state) {

        //turn off
        case ON:
          last_motorSpeed = motorSpeed; 
          motorSpeed = 0;
          next_m2_state = OFF;
          break;
       
        //turn on
        case OFF:
          motorSpeed = last_motorSpeed;
          next_m2_state = ON;
          break;
      }

      //set motor speed
      set_m2_speed(motorSpeed);

      //delay 100ms to allow button press to take effect and motor to turn on
      delay_ms(100);
    }

    //check for 2 revs
    pos = encoders_get_counts_m2();
    if ((pos % TWO_TURNS) == 0 && m2_state == ON) {
      motorSpeed = -motorSpeed;
      set_m2_speed(motorSpeed); 

      //delay 100ms to allow motor to turn on
      delay_ms(100);
    }

    //print speed
    lcd_goto_xy(0, 0);
    print("spd=");
    print_long(motorSpeed);
    print("   ");

    // all LEDs off
    red_led(0);
    green_led(0);
  
    // turn green LED on when motors are spinning forward
    if (motorSpeed > 0)
      green_led(1);
    // turn red LED on when motors are spinning in reverse
    if (motorSpeed < 0)
      red_led(1);

    //print position
    lcd_goto_xy(0, 1);
    print("pos=");
    print_long(pos);
    print("   ");

    //update state
    m2_state = next_m2_state;
  }
}
