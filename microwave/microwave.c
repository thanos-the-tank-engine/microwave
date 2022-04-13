// config vars

// maximum acceptable cook time in seconds
// default: 10 minutes (600 seconds)
static int maxtime = 600;

//  Pin definitions for the microwave components
const int light_pin = 2;
const int turntable_pin = 11;
const int fan_pin = 12;
const int magnetron_pin = 5;

#include <stdio.h>
#include <stdlib.h>
#include <cli.h>
#include "bl_gpio.h"
#include "microwave.h"
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <bl602_timer.h>

//Appliance control functions

//to ensure modularity of this code for compatibility with other appliances in the future, teh appliance-specific functions will be placed here.
//this might get moved to another file later on

void startHeating()
{
    bl_gpio_output_set(turntable_pin, 1);
    bl_gpio_output_set(magnetron_pin, 1);
    bl_gpio_output_set(fan_pin, 1); 
}

void stopHeating()
{
    bl_gpio_output_set(magnetron_pin, 0);
    bl_gpio_output_set(turntable_pin, 0);
    vTaskDelay(pdMS_TO_TICKS(2000));
    bl_gpio_output_set(fan_pin, 0);
}


//The main countdown ticker and the handling of it is right below.
//the ticker is intended to:
//* only ever have one running instance
//* be the only way to enable the magnetron
//* handle most of the microwave stuff
//please keep this in mind while working with this code. this function is implemented this way to act as a failsafe to prevent the magnetron from being left on.

//global assignment for the countdown ticker. 
TimerHandle_t countdown_handle;

//the main countdown value. please do not modify this value outside of uing "startTicker" or "stopTicker"
int main_countdown = 0;

//function to start up the countdown ticker
void startTicker(int time)
{
    if(time <= maxtime)
    {
        main_countdown = time;
        startHeating();
        xTimerStart(countdown_handle, 0);
        printf("\r\nStarting Ticker\r\n#");
    }
    else
    {
        printf("\r\nERROR: attempt to initialize timed cook for longer than maximum allowed time!");
    }
}

//function to abort the countdown
void stopTicker()
{
    stopHeating();
    xTimerStop(countdown_handle, pdMS_TO_TICKS(20));
    printf("\r\nStopping Ticker\r\n#");
}

//countdown ticker task
void CoundownTimerTIckerCallback( TimerHandle_t xTimer)
{
    if(main_countdown > 0)
    { 
        char buffer[20];
        printf("\r\nTime Remaining: ");
        printf(itoa(main_countdown, buffer, 10));
        main_countdown -= 1;

    }
    else
    {
        printf("\r\nCountdown complete, shutting down death ray!\r\n#");
        xTimerStop(countdown_handle, pdMS_TO_TICKS(20));
        stopHeating();
    }

}

//end main countdown ticker

//command to initialize a timed heating
static void TimedCookStarterFunc(char *buf, int len, int argc, char **argv)
{
    if(argc == 1)
    {
        printf("usage: timed_cook [minutes] seconds");
    }
    else{
        if(argc == 2)
        {
            startTicker(atoi(argv[1]));

        }
        if(argc == 3)
        {
            startTicker((atoi(argv[1]) * 60) + atoi(argv[2]));
        }
    }
}

//cancels the main countdown to terminate the timed cook function
static void TimerCancelFunc(char *buf, int len, int argc, char **argv)
{
   stopTicker();
}

// commands for conotrolling independent parts of the microwave
static void light(char *buf, int len, int argc, char **argv)
{
    if(argc != 2)
    {
        printf("usage: light 1/0");
        return;
    }
    int light_state = atoi(argv[1]);
    bl_gpio_output_set(light_pin, light_state);
    return;
}

static void hahamicrowavegospiiiiin(char *buf, int len, int argc, char **argv)
{
    if(argc != 2)
    {
        printf("usage: spin 1/0");
        return;
    }
    int spin_state = atoi(argv[1]);
    bl_gpio_output_set(turntable_pin, spin_state);
    return;
}

static void fan(char *buf, int len, int argc, char **argv)
{
    if(argc != 2)
    {
        printf("usage: fan 1/0");
        return;
    }
    int fan_state = atoi(argv[1]);
    bl_gpio_output_set(fan_pin, fan_state);
    return;
}

//magnetron command should be left unused in final release for user safety, if this is enabled it should only be for temporary debugging use
static void magnetron(char *buf, int len, int argc, char **argv)
{
    if(argc != 2)
    {
        printf("usage: magnetron 1/0");
        return;
    }
    int magnetron_state = atoi(argv[1]);
    bl_gpio_output_set(magnetron_pin, magnetron_state);
    return;
}

//should be run on init, puts up flashy ascii art intro, sets up the ticker function and then configures and starts the watchdog
int main_cli_init(void)
{ 
    //text looks fucked-up and misaligned because of needing to double-up backslashes to stop them being interpreted as escape sequances, these come out perfectly aligned.
    countdown_handle = xTimerCreate("countdown ticker", pdMS_TO_TICKS(1000), pdTRUE, (void *)0, CoundownTimerTIckerCallback);
    printf ("\r\n               _____  ____ _____  __  __  _  __  _    ____   _        ____  ____  __  _  _____ _____ ____  _     _     ____  ____     "); 
    printf ("\r\n              |_   _|| ===|| () )|  \\/  || ||  \\| |  / () \\ | |__    / (__`/ () \\|  \\| ||_   _|| () ) () \\| |__ | |__ | ===|| _) \\     ");
    printf ("\r\n                |_|  |____||_|\\_\\|_|\\/|_||_||_|\\__| /__/\\__\\|____|   \\____)\\____/|_|\\__|  |_|  |_|\\_\\____/|____||____||____||____/     ");
    printf ("\r\n            _  __  _  _____  ____  _     _     _  ____  ____  __  _  _____       ____  _____ _____  _     _    ____   __  _  ____  ____ ");
    printf ("\r\n           | ||  \\| ||_   _|| ===|| |__ | |__ | |/ (_,`| ===||  \\| ||_   _|     / () \\ | ()_)| ()_)| |__ | |  / () \\ |  \\| |/ (__`| ===|");
    printf ("\r\n           |_||_|\\__|  |_|  |____||____||____||_|\\____)|____||_|\\__|  |_|      /__/\\__\\|_|   |_|   |____||_| /__/\\__\\|_|\\__|\\____)|____|");   
    
    //old "microshell" intro art
    /*printf("\r\n __  __ _               ____  _          _ _ ");
    printf("\r\n|  \\/  (_) ___ _ __ ___/ ___|| |__   ___| | |");
    printf("\r\n| |\\/| | |/ __| '__/ _ \\___ \\| '_ \\ / _ \\ | |");
    printf("\r\n| |  | | | (__| | | (_) |__) | | | |  __/ | |");
    printf("\r\n|_|  |_|_|\\___|_|  \\___/____/|_| |_|\\___|_|_|");*/
    
    printf("\r\n                                        F       E      A       T      U       R       I       N       G");
    printf("\r\n __    __     __     ______     ______     ______     __     __     ______     __   __   ______        ______     __  __     ______     __         __       "); 
    printf("\r\n/\\ \"-./  \\   /\\ \\   /\\  ___\\   /\\  == \\   /\\  __ \\   /\\ \\  _ \\ \\   /\\  __ \\   /\\ \\ / /  /\\  ___\\      /\\  ___\\   /\\ \\_\\ \\   /\\  ___\\   /\\ \\       /\\ \\      "); 
    printf("\r\n\\ \\ \\-./\\ \\  \\ \\ \\  \\ \\ \\____  \\ \\  __<   \\ \\ \\/\\ \\  \\ \\ \\/ \".\\ \\  \\ \\  __ \\  \\ \\ \\\'/   \\ \\  __\\      \\ \\___  \\  \\ \\  __ \\  \\ \\  __\\   \\ \\ \\____  \\ \\ \\____ "); 
    printf("\r\n \\ \\_\\ \\ \\_\\  \\ \\_\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\__/\".~\\_\\  \\ \\_\\ \\_\\  \\ \\__|    \\ \\_____\\     \\/\\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_____\\  \\ \\_____\\"); 
    printf("\r\n  \\/_/  \\/_/   \\/_/   \\/_____/   \\/_/ /_/   \\/_____/   \\/_/   \\/_/   \\/_/\\/_/   \\/_/      \\/_____/      \\/_____/   \\/_/\\/_/   \\/_____/   \\/_____/   \\/_____/"); 
    printf("\r\n                                                    E  N  T  E  R    C  O  M  M  A  N  D\r\n");

    //configure and then enable the watchdog using the low-level timer API
    WDT_Disable();
    WDT_Set_Clock(TIMER_CLKSRC_1K, 1);
    WDT_SetCompValue(1000);
    WDT_ResetCounterValue();
    WDT_IntMask(WDT_INT, MASK);
    WDT_Enable();

    return 0;
}

//sets up the cli commands
const static struct cli_command cmds_user[] STATIC_CLI_CMD_ATTRIBUTE = {
    //individual components on/off
    {"light", "turn light on/off", light},
    {"spin", "turn the spinny plate on and off", hahamicrowavegospiiiiin},
    {"fan", "turn the fan on and off", fan},
    //only here for debugging, leave this commented out in any release that could touch an end user.
    //{"magnetron", "death ray", magnetron},

    //actual cooking commands
    {"timedcook", "cook food for a set time period in minutes and seconds", TimedCookStarterFunc},
    {"canceltimer", "cancels the main countdown timer", TimerCancelFunc}


};

//uses the tick hook to pet the watchdog so that if for any reason freertos locks up the watchdog pushes a hard reset.
//this ensures that the software is constantly fail-safe; there is no condition where the magnetron is enabled and not going to be automatically cut off.
//the routine controlling it is on a constant countdown and the gpio states are set on boot. no method to enable the magnetron without starting the timer should be available.
void vApplicationTickHook( void )
{
WDT_ResetCounterValue();
}