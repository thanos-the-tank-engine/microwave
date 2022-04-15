//we need a type from this header file to make the pwm driver happy
#include <bl602_pwm.h>

//  MicrowaveOS compile-time options
// these are things that we don't really need to expose to an end user,
// but they'll be here for power users and devs, so that if we need to tweak things all the values will be in one place.

//  Pin definitions for the microwave components
static const int light_pin = 2;
static const int turntable_pin = 11;
static const int fan_pin = 12;
// IMPORTANT NOTE: make absolutely sure that if you change this pin to another one that the new pin does not go high during boot or in flashing mode!!
static const int magnetron_pin = 5;
/* PWM channel used for the magnetron
    options:
    PWM_CH0  -  GPIO pins 0, 5, 10, 15
    PWM_CH1  -  GPIO pins 1, 6, 11, 16
    PWM_CH2  -  GPIO pins 2, 7, 12
    PWM_CH3  -  GPIO pins 3, 8, 13
    PWM_CH4  -  GPIO pins 4, 9, 14          */
static const PWM_CH_ID_Type magnetron_pwm_ch = PWM_CH0;


// maximum acceptable cook time in seconds
// default: 10 minutes (600 seconds)
static int maxtime = 600;