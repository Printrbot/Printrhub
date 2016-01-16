# Introduction

This is the firmware for the Printrhub, the LCD based Controller for Printrbots.

# Requirements

This project is based on teensy-cmake (https://github.com/xya/teensy-cmake) and can be used with any CMake compatible IDE (or the commandline). It’s working great with CLion.

If you want to open this project in Arduino IDE, just copy the Printrhub directory from sketches/Teensy in your Arduino Documents folder.

# Setup

This setup is only described for Macs. If you want to compile on Windows/Linux you will have to adjust directories (see below). You will need to follow these steps to get this project compiled:

## 1.) Install Arduino IDE

Download Arduino IDE Version 1.6.7 from https://www.arduino.cc/en/Main/Software. It’s important to choose version 1.6.7! Rename the file Arduino.app to Arduino_1.6.7 and copy it in the /Applications directory of your Mac.

## 2.) Install Teensyduino

Download Teensyduino (https://www.pjrc.com/teensy/td_download.html) and and install Teensyduino in /Applications/Arduino_1.6.7 (this is the file you copied in the previous step). During install select all available libraries to be installed, too.

## 3.) Adjust Paths

If you followed the steps below (on a Mac) your setup should now be complete. If you chose other names or directories, you will have to adjust variables in this file: __cmake/teensy-arm.toolchain.cmake__.

# Targets

This CMake project is configured with these targets:

* **Teensy_Printrhub**: This builds the project (core and firmware)
* **Teensy_Printrhub_Firmware**: This just builds the firmware (your code)
* **Teensy_Printrhub_Core**: This builds the Teensy core library
* **Teensy_Printrhub_Upload**: When selecting this target the firmware and core is compiled, linked and uploaded to the Teensy. Watch compiler output when to press the Program button on the Teensy.

# Uploading to Teensy

Build the Teensy_Printrhub_Upload target (select as Build Target in CLion or similar IDE or name it when compiling on the command line). When build process has finished you will see something like this:

```
Teensy Loader, Command Line, Version 2.0
Read „/Users/pschuster/Documents/Programming/Printrhub/Firmware/build/bin/Teensy_Printrhub.elf.hex“: 31648 bytes, 12.1% usage
Waiting for Teensy device…
 (hint: press the reset button)
```

Make sure your Teensy is connected via USB. Press the program button on Teensy and the upload should start:

```
Found HalfKay Bootloader
Read „/Users/pschuster/Documents/Programming/Printrhub/Firmware/build/bin/Teensy_Printrhub.elf.hex“: 31648 bytes, 12.1% usage
Programming………………………….
Booting
[100%] Built target Teensy_Printrhub_Upload
```

That’s it. Teensy should restart and the new firmware is applied.

# Importing libraries

In Arduino IDE it’s enough to just include the header of the library. But CMake will have to know which libraries to link. To do that, add the library by adding this to the file __sketches/Teensy/Printrhub/CMakeLists.txt__:

```
import_arduino_library(Bounce)
```

Just below or above the other import statements.

CMake will search for libraries in the ARDUINO_LIB_ROOT folder.

# More Info and References

Please read the excellent documentation at teensy-cmake (https://github.com/xya/teensy-cmake). More Info on CLion can be found at www.jetbrains.com.

# Problems and Feedback

If you have any issues please place an issue. We are very responsive!