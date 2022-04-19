/*  Headers for our GPIO implemtation.
    for the sake of cleanliness and allowing use of i/o functions in multiple files, this has been set up as a sort of library.
    while this makes things more portable, that was not the main goal.
*/


//to be called during init, sets up everything for the GPIOs including PWMs and everything.
void init_gpio_config(void);

//sets a gpio pin.
void gpio_set(int pin, int state);

//starts up a PWM channel at the set power level
void pwm_start(int ch, int level);

//stops a pwm channel
void pwm_stop(int ch);