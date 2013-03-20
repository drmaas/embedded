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
  long iteration = 10000;
  long test_iteration = 1000;

  //TODO: possible only call ticks_to_microseceonds once after all times are recorded.
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

  //calculate time to loop 1000 times in microseconds
  //result is about .924 ms
  //so 1082 iterations take 1 ms
  //for some reason this is actually 2 ms, so divide by 2
  long wcet = (loop2time - loop1time)/iteration;
  long one_ms = (1000*test_iteration)/(wcet*2); 

  clear();
  lcd_goto_xy(0,0);
  printf("time:%li",wcet);
  lcd_goto_xy(0,1);
  printf("1 ms iter:%li",one_ms);
}
