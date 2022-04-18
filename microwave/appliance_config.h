//we need a type from this header file to make the pwm driver happy
#include <bl602_pwm.h>

//  MicrowaveOS compile-time options
// these are things that we don't really need to expose to an end user,
// but they'll be here for power users and devs, so that if we need to tweak things all the values will be in one place.
// this config will also likely end up being not just board-specific but alo implementation specific;
// it will be used to define if the microwave or toaster oven subsystems should be used, or even if it should use both for hybrid devices.


//appliance details, not yet implemented but planned for: eventually.
//some microwaves have a resistive heating element inside, this one's gonna end up being fun to implement. thank god toaster oven support is planned anyway.
static const int heating_element_present = 0;
static const int magnetron_present = 1;
static const int pin_to_consolidate = 3; //thing that we'll merge with the magnetron, default is the magnetron cooling fan.




//  Pin definitions for the appliance components
static const int light_pin = 2;
static const int turntable_pin = 11;
static const int fan_pin = 12;
//pins to assign to resistive heating elements, supports dual zone heating if present. not yet implemented.
static const int top_element_pin = 0;
static const int bottom_element_pin = 0;

// IMPORTANT NOTE: make absolutely sure that if you change this pin to another one that the new pin does not go high during boot or in flashing mode!!
// also be sure to set the correct PWM channel too.
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
// lowest acceptable power level in percentage
//default: 20
static int minpower = 20;

