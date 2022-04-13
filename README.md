#BL602 Based Microwave Retrofit Controller

This is a project that aims to make a powerful open source framework for converting your existing kitchen appliances in to internet connected smart appliances using the low-cost BL602 microcontroller. this offers several benefits over buying a commercial smart appliance. you have full control over the hardware and code being used, it doesn't rely on any servers, or if it does you're the one hosting it, and it doesn't phone home to sell user data.

This project aims only to create the hardware and software to run the appliance and provide a backend for controlling it. if you want a fancy graphical frontend for it or anything like that feel free to make one yourself. 

the software and hardware contained in this project should be considered to be in a very early alpha state. the hardware hasn't even been designed yet and the code is still barely just a proof-of-concept at this point. progress will likely be slow so feel free to contribute if you want things done. I'm an amature embedded programmer and this chip isn't too well documented, so it takes a good bit of time to figure out how to impleent things.

This software and hardware is being released as public domain. feel free to use it however you like, though it would be preferred if you credit me in anything you make using it, and it would be nice if you contribute any improveents you make.

#Planned Hardware
Display: 128x128 SSD1327 OLED panel
Buttons: 8x8 matrixed keypad, possibly with dedicated controller to save pins and cpu cycles
MCU: BL602 or BL604 if more pins are needed
Sensors: IR thermometer, ambient temperature and humidity with either a DHT11 or BME280
Switching: Current plan is to just use mechanial relays, might change to solid state later on down the line.
Memory: external flash chip housing cooking mode database and any LVGL assets that can't be fit in to the bl602's internal flash, if the extra space is needed.
Hardware designs have not yet been created, this will be done when the backend is a bit more complete.

#Building a "devkit"
the hardware I am currently using to work on this project is rather simple. it is a cheap 4-channel relay board connected to all of the components in the microwave that is powered and controlled by a Pine64 PineCone bl602 EVB.
The current software expects 4 components: a light, a turntable, a fan, and a magnetron.
The way to hook it up is to remove the existing control board from the appliance and then cut the power wires for each part, and then connect them to a relay on the relay board. the pin layout I chose is as follows:

Light: GPIO 2
Turntable: GPIO 11
Fan: GPIO 12
Magnetron: GPIO 5

these pins were chosen based on how they react during startup to avoid having the magnetron get pulsed unintentionally. the pin layout may change in the future.

#How To Build

linux

requirements: bl_iot_sdk
step 1: run the command `export BL60X_SDK_PATH path/to/sdk`
step 2: run make (if it errors out but microwave.bin is in build_out just pretend it didn't error)
step 3: flash it to the bl602 using your flashing utility of choice

windows

lol idk

#TODO:

Backend:
*implement wifi connectivity
*implement SSH and Telnet access to the cli
*implement sensors and sensor cooking modes
*implement cooking mode database and loader

Frontend:
*implement display driver
*implement keypad driver
*set up LVGL
*design entire UI

Other:
*name project
*rewrite sections of the source code that were ripped from the bl_iot_sdk demos
*ensure code is modular enough to be ported over to other appliances without too much work
