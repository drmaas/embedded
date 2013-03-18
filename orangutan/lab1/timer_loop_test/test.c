#include <pololu/orangutan.h>

// useful stuff from libc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main()
{
  lcd_init_printf();
  clear();
  lcd_goto_xy(0,0);
  printf("Calculating...");

  volatile int i,j;

  // Measure time for loop execution (without test code)
  long iteration = 20000;
  long test_iteration = 1000;

  //time to loop million times
  long time1 = ticks_to_microseconds(get_ticks());
  for (i=0; i < iteration; i++);
  long time2 = ticks_to_microseconds(get_ticks());
  long loop1time = time2 - time1;

  long time3 = ticks_to_microseconds(get_ticks());
  for (i=0; i < iteration; i++) {
    for(j=0;j<test_iteration;j++);
  }
  long time4 = ticks_to_microseconds(get_ticks());
  long loop2time = time4 - time3;

  //calculate time to loop 1 time in microseconds
  //result is about 9.243 ms
  long wcet = (loop2time - loop1time)/test_iteration;

  clear();
  lcd_goto_xy(0,0);
  lcd_goto_xy(0,0);
  printf("time:%li",wcet);
}
