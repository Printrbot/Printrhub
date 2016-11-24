# Firmware for MK20DX256LHV7 (Teensy 3.x) MCU

Printrhub consists of two MCUs on one board. The first is the well know ESP-8266 MCU, implemented as a ESP-12E module on the PCB. The second MCU is MK20DX256VLH7, better known as Teensy 3.1/3.2.

Teensy MCU is responsible for these features:

* Reads and writes from SD card
* Controls Display, Touch and printer
* Communicates with ESP-12E module to download project index files and G-Codes from Printrbot cloud

You will find the firmware for the MCU on Printrhub board in this folder (mk20). This is what you need to compile and install the firmware:

* Latest PlatformIO IDE (http://platformio.org)
* CLion (optional) (https://www.jetbrains.com/clion/)
* Segger J-LINK or similar SWD compatible debug probe

## Documentation

One of our team members has written an extensive documentation and behind the scenes report about the development of the Printrbot Simple 2016. We advise you to read this before digging into this code to give you some context and advise.

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

## Using PlatformIO IDE to compile MK20 firmware

Download the latest PlatformIO IDE and install it. Open the mk20 folder in your PlatformIO IDE and compile the code. 

## Uploading firmware

There are two options to upload new firmware to the device:

* Using existing over the air update mechanism (see ESP firmware in this repository)
* SWD compatible ARM debug probe (like Segger J-LINK) to upload code via Serial Wire Debug (SWD)
  
### Over the Air updates
  
  If you don't have the necessary hardware (debug probe) to upload code via SWD then this is the easiest method. You will need to modify the ESP firmware. This is an excerpt of the file web_server.cpp and sets up a GET request that is watching for a /update_mk20 event and starts a firmware update. 
  
  ```
    webserver.addOptionsRequest("/update_mk20");
    server.on("/update_mk20", HTTP_GET, [](AsyncWebServerRequest *request) {
  	String mk20FirmwareFile("/mk20.bin");
  	FirmwareUpdateInfo *updateInfo = Application.getFirmwareUpdateInfo();
  	if (updateInfo != NULL) {
  	  DownloadFileToSPIFFs *downloadMK20Firmware = new DownloadFileToSPIFFs(updateInfo->mk20_url, mk20FirmwareFile);
  	  MK20FirmwareUpdate *mk20UpdateFirmware = new MK20FirmwareUpdate(mk20FirmwareFile);
  	  downloadMK20Firmware->setNextMode(mk20UpdateFirmware);
  	  Application.pushMode(downloadMK20Firmware);
  	  request->send(200, "text/plain", "\nupdate of MK20 started, please wait...\n\n");
  	} else {
  	  request->send(200, "text/plain", "\nupdate of MK20 failed, please wait until firmware data have been loaded and try again...\n\n");
  	}
    });
  ```
  
  However, this code loads the firmware provided by the Printrbot Cloud. You will want to user your own binary file. Best way is to modify it this way:
   
     ```
       webserver.addOptionsRequest("/update_mk20");
       server.on("/update_mk20", HTTP_GET, [](AsyncWebServerRequest *request) {
     	String mk20FirmwareFile("/mk20.bin");
     	FirmwareUpdateInfo *updateInfo = Application.getFirmwareUpdateInfo();
     	if (updateInfo != NULL) {
     	  DownloadFileToSPIFFs *downloadMK20Firmware = new DownloadFileToSPIFFs("http://WWW.YOURDOMAIN.COM/YOURFIRMWARE.BIN", mk20FirmwareFile);
     	  MK20FirmwareUpdate *mk20UpdateFirmware = new MK20FirmwareUpdate(mk20FirmwareFile);
     	  downloadMK20Firmware->setNextMode(mk20UpdateFirmware);
     	  Application.pushMode(downloadMK20Firmware);
     	  request->send(200, "text/plain", "\nupdate of MK20 started, please wait...\n\n");
     	} else {
     	  request->send(200, "text/plain", "\nupdate of MK20 failed, please wait until firmware data have been loaded and try again...\n\n");
     	}
       });
     ```
     
     This will use the standard over the air updates but will use the URL you provide to flash MK20.
     
The workflow would be this:
     
     * Compile MK20 firmware and generate a bin file from the hex file (see hextobin.sh) for details on how to do that
     * Upload your firmware binary to your own webspace or local webserver
     * Modify ESP firmware and flash ESP
     * Load this URL in your browser to receive some helful logs (http://IPADDRESS_OF_PRINTER/events)
     * Start up your printer and call this URL in your browser (http://IPADDRESS_OF_PRINTER/update_mk20)
     * ESP will download your firmware and install it. Your display will turn off.
     
### Uploading code via SWD
      
If you want to upload via SWD connect your probe to SWD_IO, SWD_CLK, GND and RESET_B. You will also need to connect your VREF to 3.3V. This way your debug probe can sense the correct logic level. See your manual for details on the procedure. All necessary signals are exposed in the test points in the middle of the board.

## Using CLion to compile and upload ESP firmware

It's possible to use the awesome CLion IDE and we internally used that for development. CLion will use PlatformIO in the background but will seamlessly integrate it. You get a superior IDE but without the CMake hassle. After you have installed PlatformIO and CLion open a terminal and navigate to this mk20 folder. Use this command to let PlatformIO setup a CLion IDE project for you.

`platformio init --ide clion`

Open the folder mk20 in CLion and you should have a nice, already setup project in CLion. Use the different targets available like PLATFORMIO_BUILD to compile the project. Use the Build command to run the target process. 

You will need 2016.2 or the latest 2016.3 of CLion for remote debugging via SWD. 

## Support

There is no official support for this, but please drop an Issue and we'll have a look. If you find bugs or add enhancements, please let us know and send a pull request.

## License

Copyright (c) 2016 Printrbot Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Credits

Contains code or portions from these great libraries

* https://github.com/bakercp/PacketSerial
* https://github.com/PaulStoffregen/ILI9341_t3
* https://github.com/PaulStoffregen/ILI9341_fonts

Developed in cooperation with Phillip Schuster (@appfruits) from Appfruits (http://www.appfruits.com).

## Support us

Printrbot invests time and resources providing this open source code, please support Printrbot and open-source hardware by purchasing products from Printrbot (http://www.printrbot.com)!