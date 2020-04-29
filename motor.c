#include "bcm2837.h"
#include "TB6612FNGD.c"

#include "map_dev.c"
#include <unistd.h>
#include <time.h>

// Do not use Pin 18 for anything other than PWM0
#define PWA                 0     // On Pin18

#define PHOTORESISTOR_PIN_L 17
#define PHOTORESISTOR_PIN_R 16

#define AI1                 26
#define AI2                 19

#define FREQ                50
#define STANDBY             25
#define STRAIGHT_DC         50
#define TURNING_DC          100


TB6612FNGDc motor1;

void motor1_init() {
  motor1.pin1 = AI1;
  motor1.pin2 = AI2;
  motor1.pwm_num = PWA;
  motor1.freq = FREQ;

  setup(&motor1);
}

void photo_resistor_init() {
  gpio_set_dir(PHOTORESISTOR_PIN_L, gpio_in);
  gpio_set_dir(PHOTORESISTOR_PIN_R, gpio_in);
}

int main() {
  map_dev();
  motor1_init();
  photo_resistor_init();

  gpio_set_dir(25, gpio_out);
	gpio_set_level(25, gpio_high);

  struct timespec start, end;
  double latency[10];
  double avg_latency = 0;
  double throughput[10];
  double avg_throughput = 0;

  while (1) {
    clock_gettime( CLOCK_MONOTONIC, &start );
    for ( int k = 0; k < 1000; k = k + 100 ){             // read 100, 200, 300, ... , 900 data and measure timing respectively 

      int l = gpio_get_level( PHOTORESISTOR_PIN_L );
      int r = gpio_get_level( PHOTORESISTOR_PIN_R );

      if ( l == gpio_low ) {
       if ( r == gpio_low ) {
         forward( &motor1, STRAIGHT_DC );
       }
       else {
         forward( &motor1, TURNING_DC );
       }
      }
	    else if ( l == 1 ) {
        stop( &motor1, 0 );
      }

      clock_gettime( CLOCK_MONOTONIC, &end );
      double time;
      time = ( end.tv_sec - start.tv_sec ) + ( end.tv_nsec - start.tv_nsec );
      latency[k/100] = time;
    }

    for ( int i = 0; i < 10; i ++ ) {
      throughput[i] = 100 * i / latency[i];
      avg_throughput = avg_throughput + throughput[i];
      avg_latency = avg_latency + latency[i];
    }

    avg_throughput = avg_throughput / 10;
    avg_latency = avg_latency / 10;

    printf( "the average latency is: %f ns\n", avg_latency );
    printf( "the average throuput is: %f op/s\n", avg_throughput );
    break;

    // int l = gpio_get_level( PHOTORESISTOR_PIN_L );
    // int r = gpio_get_level( PHOTORESISTOR_PIN_R );
    // /*
    // if ( l == gpio_high || r == gpio_high) {
    //   printf("Got a high\n");
    //   forward(&motor1, 0 );
    // } else {
    //   printf("Got lows\n");
    //   stop(&motor1, TURNING_DC);
    // }
    // */
    // if ( l == gpio_low ) {
    //  if ( r == gpio_low ) {
    //    forward( &motor1, STRAIGHT_DC );
    //  }
    //  else {
    //    forward( &motor1, TURNING_DC );
    //  }
    // }
	  // else if ( l == 1 ) {
    //   stop( &motor1, 0 );
    // }
  }
}