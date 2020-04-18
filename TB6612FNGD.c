#include "bcm2837.h"
//             TB6612FNGDc control board control logic
// AIN1    AIN2    BIN1    BIN2    PWMA    PWMB    AO1/AO2
// -----------------------------------------------------------
// 1       0       1       0       1       1       positive
// 0       1       0       1       1       1       reverse
// 1       1       1       1       1       1       brake
// 0       0       0       0       1       1       free stop
// x       x       x       x       0       0       brake

// Use 500MHz with a dividor of 10
// so CLK = 50MHz, Period = 20ns
#define KHz2M(freq)             50 * 1000 / freq
#define ms2M(period)            period * 1000 / 20
#define dcKH2N(freq, dc)        KHz2M(freq) * dc / 100
#define dcms2N(period, dc)      ms2M(period) * dc / 100

typedef struct {
    // pin1: GPIO connected to AI1 or BI1;
    // pin2: GPIO connected to AI2 or BI2;
    // pwm_num: Use PWM0 or PWM1;
    int pin1;
    int pin2;
    int pwm_num;
    int freq;
} TB6612FNGDc;

void setup(TB6612FNGDc *motor) {   
    gpio_set_dir(motor->pin1, gpio_out);
    gpio_set_dir(motor->pin2, gpio_out);
    gpio_set_func(18, gpio_alt5);
    
	pwm_set_clk(10);
	pwm_set_period(motor->pwm_num, KHz2M(motor->freq));
	pwm_set_width(motor->pwm_num, 0);
	pwm_enable(motor->pwm_num);
}

void forward(TB6612FNGDc *motor, int duty_cycle ) {
    gpio_set_level(motor->pin1, gpio_high);
    gpio_set_level(motor->pin2, gpio_low);
    pwm_set_width(motor->pwm_num, dcKH2N(motor->freq, duty_cycle));
}

void stop(TB6612FNGDc *motor, int duty_cycle ) {
    gpio_set_level(motor->pin1, gpio_low);
    gpio_set_level(motor->pin2, gpio_low);
    pwm_set_width(motor->pwm_num, dcKH2N(motor->freq, duty_cycle));
}