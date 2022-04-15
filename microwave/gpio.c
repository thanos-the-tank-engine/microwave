/*

MicrowaveOS gpio functions
this file contains the tandardized functions for gpio handling within MicrowaveOS

Like the rest of MicrowaveOS this file is released to the public domain. you may do with it whatever you please.
*/

#include "appliance_config.h"
#include <bl602_pwm.h>
#include <bl602_glb.h>

// function that sets up the PWM for the magnetron pin.
// In this case we are PWMing on a timescale of a few seconds to emulate the way a conventional microwave does it.
//this *could* be done in software, but I didn't feel like programming that, and this way it should be more reliable anyway.
void conf_pwm(PWM_CH_ID_Type ch)
{
PWM_CH_CFG_Type cfg; 

cfg.ch = ch;
//clock is set up to go about every 40 milliseconds, not exact because that would make this a bit more complicated, and it doesn't really matter how precise this is.
//this enables our abuse of the PWM controller. the slow clock gives us the desired timescale
cfg.clk = PWM_CLK_32K;
cfg.clkDiv = 1312;
//these two are undocumented, but I believe stopmode decides if you want it to finish the cycle it's in already when disabled, and polarity seems to allow you to invert the output.
cfg.stopMode = PWM_STOP_ABRUPT;
cfg.pol = PWM_POL_NORMAL;
//we actually have quite a fancy setup here with dual thresholds nd variable perios, but we're using neither of those features really.
//period is set to 100 so we can just yeet a percentage at threshold 2, and threshold 1 is just set at the beginning of the period, so it's basicaly just ignored.
cfg.period = 99;
cfg.threshold1 = 0;
cfg.threshold2 = 100;
//set to zero because we aren't using the interrupts
cfg.intPulseCnt = 0;
//ensure channel is disabled first, then push he config to it
PWM_Channel_Disable(ch);
PWM_Channel_Init(&cfg);
}


//Implementation of the function to configure a gpio pin. we could use the hal function but tht would make this incompatible with the upcoming rewrite removing all of the high level sdk functions.
void conf_gpio(int pin, int function, int mode)
{
    GLB_GPIO_Cfg_Type cfg;
    // assume pulldowns for fail-safe on all of the control lines.
    cfg.drive=0;
    cfg.smtCtrl=1;
    cfg.gpioPin = pin;
    cfg.pullType = GPIO_PULL_DOWN;
    cfg.gpioMode = mode;
    cfg.gpioFun = function;
    GLB_GPIO_Init(&cfg);
}

//function to turn gpio pins on and off
void gpio_set(int pin, int state)
{
    GLB_GPIO_Write((GLB_GPIO_Type)pin, state ? 1 : 0);
}

//configures all of the pins for us on startup
void init_gpio_config()
{
    conf_gpio(turntable_pin, 11, GPIO_MODE_OUTPUT);
    conf_gpio(fan_pin, 11, GPIO_MODE_OUTPUT);
    conf_gpio(light_pin, 11, GPIO_MODE_OUTPUT);
    conf_gpio(magnetron_pin, 8, GPIO_MODE_OUTPUT);
    conf_pwm(magnetron_pwm_ch);
}