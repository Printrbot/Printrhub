# Firmware for ESP-12E module

Printrhub consists of two MCUs on one board. The first is the well know ESP-8266 MCU, implemented as a ESP-12E module on the PCB. The second MCU is MK20DX256VLH7, better known as Teensy 3.1/3.2.

ESP is responsible for these features:

* Flashes MK20 MCU with latest firmware using ARMs SWD protocol
* Requests and downloads from the web
* Initial setup with WiFi and connecting your printer to the Printrbot Cloud

You will find the firmware for the ESP module on Printrhub board in this folder (esp). This is what you need to compile and install the firmware:

* Latest PlatformIO IDE (http://platformio.org)
* CLion (optional) (https://www.jetbrains.com/clion/)
* FTDI cable to transfer firmware to ESP module (for example this one https://www.sparkfun.com/products/9873)

## Getting started

Connect the FTDI header to the connection port marked FTDI on Printrhub and make sure it's connected in the right direction. If you use Sparkfuns FTDI breakout the IC on the breakout board should point to the Printrhub board.

## Uploading firmware to ESP

Before uploading the ESP firmware you will need to set ESP in DFU mode. You will need to do that before each upload cycle.

Press and hold the button named GPIO0 on the Printrhub board and press the RESET button on the Printrhub board. Release the GPIO0 button. The small LED on the bottom right side of the Printrhub should lightly dim now
indicating ESP is in DFU mode now.

## Using PlatformIO IDE to compile and upload ESP firmware

Download the latest PlatformIO IDE and install it. Open the esp folder in your PlatformIO IDE and compile the code. 

Make sure PlatformIO IDE uses the right USB connection (to the FTDI cable or breakout board) and upload the code. It will take some time.

## Using CLion to compile and upload ESP firmware

It's possible to use the awesome CLion IDE and we internally used that for development. CLion will use PlatformIO in the background but will seamlessly integrate it. You get a superior IDE but without the CMake hassle.
After you have installed PlatformIO and CLion open a terminal and navigate to this esp folder. Use this command to let PlatformIO setup a CLion IDE project for you.

`platformio init --ide clion --board esp12e`

Open the folder esp in CLion and you should have a nice, already setup project in CLion. Use the different targets available like PLATFORMIO_BUILD to compile the project and PLATFORMIO_UPLOAD to build and upload the project. 
Use the Build command to run the target process. Before uploading make sure your FTDI cable/breakout board is connected and ESP is in DFU mode (see above).

## Initial setup

If you applied the firmware you should see a new WiFi network named "Printrbot" on your phone or Mac/PC. Connect to that and open http://printrbot.local in your browser. You should see an initial setup screen
where you can enter your WiFi credentials of your local network.

Once that is done, ESP restarts and tries to connect to the local network. Wait a few minutes as ESP will detect that MK20 does not have a firmware installed and will automatically dowmload the latest MK20 firmware
from the web and flash MK20. After about 3-4 minutes Printrhub display will turn on and Printrhub is ready to explore

## Special considerations

ESP and MK20 use a direct connection based on UART and a custom protocol that we developed: CommStack. As CommStack uses the same Serial connection as FTDI cable uses to upload the firmware it's sometimes necessary to
 keep MK20 disabled while uploading the firmware. If you get strange upload errors and are sure ESP module is in DFU mode you have to short RESET_B (named RES_B on the Printrhub board) to GND. It's easy to do that as
 Printrhub has test points you can use to do that. If you do more constant development you should solder a wire to RES_B and GND and connect them together before uploading a firmware to ESP.
 
 MK20 detects if you want to upload a firmware to ESP by sensing the GPIO0 signal and stop any Serial communication through CommStack, but if MK20 firmware is not running correctly which can sometimes happen during 
 development you will have to stop it running.

## Support

There is no official support for this, but please drop an Issue and we'll have a look.

## License

Copyright (c) 2016 Printrbot Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Credits

Contains code or portions from these great libraries

* https://github.com/scanlime/esp8266-arm-swd
* https://github.com/bakercp/PacketSerial
* https://github.com/esp8266/Arduino
* https://github.com/me-no-dev/ESPAsyncTCP.git
* https://github.com/me-no-dev/ESPAsyncWebServer.git
* https://github.com/bblanchon/ArduinoJson.git

Developed in cooperation with Phillip Schuster (@appfruits) from Appfruits (http://www.appfruits.com).

## Support us

Printrbot invests time and resources providing this open source code, please support Printrbot and open-source hardware by purchasing products from Printrbot (http://www.printrbot.com)!