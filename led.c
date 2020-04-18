#include "bcm2837.h"
#include "map_dev.c"

#include <stdio.h>
#include <unistd.h>


static void gpio_test_out(uint8_t pin, gpio_level level) {
	gpio_set_dir(pin, gpio_out);
	gpio_set_level(pin, level);
}

static void gpio_test_in(uint8_t pin) {
	gpio_set_dir(pin, gpio_in);
	if (gpio_get_level(pin) == gpio_high) printf("Pin %d -> High\n", pin);
	else printf("Pin %d -> Low\n", pin);
}

static void pwm_test() {
	pwm_set_clk(10);
	pwm_set_period(0, 1024);
	pwm_enable(0);
	gpio_set_func(18, gpio_alt5);

	int bright;
	while(1) {
    for (bright = 0 ; bright < 1024 ; ++bright) {
			pwm_set_width(0, bright);
      usleep(1000);		
    }
    for (bright = 1023 ; bright >= 0 ; --bright) {
			pwm_set_width(0, bright);
      usleep(1000) ;
    }
  }
}

int main() {
	if (map_dev() != 0) return 1;
	pwm_test();
	// gpio_test_in(24);
	// gpio_test_in(25);
	// gpio_test_out(8, gpio_high);
	// gpio_test_out(7, gpio_high);
}