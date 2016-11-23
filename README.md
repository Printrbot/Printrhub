# Printrhub

Welcome and thank you for your interest in our Printrhub technology. In this repository you will find everything you need to build upon the technology we invented. You may use the code and hardware to build your own devices or to improve your own Simple 2016. Please let us know about your projects and post issues or pull request to help us improve this project and code base.

## Purpose of the Printrhub

The Printrhub is the display component of our 3D printer Simple 2016 (http://printrbot.com/new-2016-simple/) and thus is the main user interface. The user can select, start, modify and pause prints or setup the printer with it. A fluid user interface controlled by touch as you know it from your smartphone let's anyone can start a print. The Hub also controls the printer and handles communication with the Printrbot cloud.

The Printrhub contains these components that form the final hardware

* ESP-12E module for WiFi connection and Over the Air updates
* MK20DX256LHV7 (Teensy 3) MCU is the main processor for handling the display, touch, SD card, controls the printer and ESP-12E
* 2.8 inch TFT display with 320x240 pixels
* Capacitive touch screen
* microSD card slot

As our board hosts two MCUs we need two different firmware, one for the ESP-12E module and the other one for the main MCU.

## Organization of this repository

/esp: The firmware for the ESP-12E WiFi module
/mk20: Contains the firmware for the main processor
/pcb: Revisions 0.1, and 0.4 (final revision) of the PCB as Eagle and Copper files (for BOM and 3D views)
/utils: The image tool that we use to build the ui.min file that contains all images used by the firmware combined in one file
/sdcard: The initial content of the SD card that we ship with your printer (contains firmware images and root folder structure)
 
## Documentation
  
We have developed the hub in cooperation with Phillip Schuster (http://www.appfruits.com). He has written some documentation and behind the scenes reports that you should have read before working with our code and hardware.

### Behind the scenes of the Printrbot Simple 2016

How we started development and why we think you will learn something about product development in general.
http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/

### Display System Explained

Expains the display and application framework in great detail. Explains the application flow, how to pseudo code multi threading and how implement a fast fluid and responsive user interface in a device capable of only 64 KB of RAM and 72 MHz.
http://www.appfruits.com/2016/11/printrbot-simple-2016-display-system-explained/

### CommStack explained

We invented CommStack to handle the communication between the MK20 and the ESP-12E module. It's fast, robust, bidirectional and features a nice request/response based system that enables two or more MCUs to communicate without having to handle all the little details.
http://www.appfruits.com/2016/11/printrbot-simple-2016-commstack-explained/

### Hardware explained

Describes the hardware in detail. Provides schematics, layout and details about the hardware we developed for the Printrbot hub.
http://www.appfruits.com/2016/11/printrbot-simple-2016-hardware-pcb-explained/

## Credits

Contains code or portions from these great libraries

* https://github.com/bakercp/PacketSerial
* https://github.com/PaulStoffregen/ILI9341_t3
* https://github.com/PaulStoffregen/ILI9341_fonts
* https://github.com/scanlime/esp8266-arm-swd
* https://github.com/esp8266/Arduino
* https://github.com/me-no-dev/ESPAsyncTCP.git
* https://github.com/me-no-dev/ESPAsyncWebServer.git
* https://github.com/bblanchon/ArduinoJson.git

Developed in cooperation with Phillip Schuster (@appfruits) from Appfruits (http://www.appfruits.com). Big thanks to Paul J. Stoffregen (https://www.pjrc.com/teensy/index.html) and his amazing Teensy project. It served as a great starting point and his amazing display driver for ILI9341 based display made our modern, fluid user interface possible.

## Support us

Printrbot invests time and resources providing this open source code, please support Printrbot and open-source hardware by purchasing products from Printrbot (http://www.printrbot.com)!
