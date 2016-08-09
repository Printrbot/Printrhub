# Introduction

This is the firmware for the Printrhub, the LCD based Controller for Printrbots.

# Requirements

To compile this project you will need platformio: http://platformio.org/

It also requires working ESP8266 module flashed with current firmware (so do this first!)

# Setup

After you download the project, cd into the directory where you extracted the files execute:

platformio run

This will compile the project. You will need to convert compiled hex file to bin before it can be flashed to MK20.
You can do this with following command (mac)

```
/Applications/Arduino_1.6.7.app/Contents/Java/hardware/tools/arm/bin/arm-none-eabi-objcopy --input-target ihex --output-target binary firmware.hex firmware.bin“
```

Once you have a bin file, you can flash it by opening following url in browser (or curl from command line):

```
curl http://printrbot.local/updatemk20?url=http://YOURSERVER.com/firmware/mk20.bin
```

You will need to change YOURSERVER with the correct url where you uploaded the bin file

There are two shell scripts included in the root folder that you can use to automate the deployment process (deploy.sh and hextobin.sh), just remember to update the paths.
