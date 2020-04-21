#include "bcm2837.h"
#include "TB6612FNGD.c"

#include "map_dev.c"
#include <unistd.h>

// Do not use Pin 18 for anything other than PWM0
#define PWA                 0     // On Pin18

#define PHOTORESISTOR_PIN_L 17
#define PHOTORESISTOR_PIN_R 16

#define AI1                 26
#define AI2                 19

#define FREQ                50
#define STANDBY             25
#define STRAIGHT_DC         25
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

  while (1) {
    int l = gpio_get_level( PHOTORESISTOR_PIN_L );
    int r = gpio_get_level( PHOTORESISTOR_PIN_R );
    /*
    if ( l == gpio_high || r == gpio_high) {
      printf("Got a high\n");
      forward(&motor1, 0 );
    } else {
      printf("Got lows\n");
      stop(&motor1, TURNING_DC);
    }
    */
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
  }
}