# Printrhub PCB

This repository contains two revisions of the PCB of the Printrhub. See main folder for more details about the project.

## Documentation and details

This PCB has been developed in cooperation by Phillip Schuster (http://www.appfruits.com). He has written a blog post about these PCBs and documents the details of it.
http://www.appfruits.com/2016/11/printrbot-simple-2016-hardware-pcb-explained/

## Revisions

This folder contains two revisions:

### Revision 0.1

This has been the first PCB we have created and contains the Teensy bootloader chip and an USB port that makes it easy to flash new firmware. It's basically just a bigger Teensy with more components. The firmware supports this revision, but you will need to set the correct #define in the HAL.h file in both firmwares

### Revision 0.4

This is the final revision we ship with the Simple 2016. Firmware must be flashed via SWD or our Over the Air update system.

## EAGLE and Copper files

These files can be opened with EAGLE, but you will need at least a Hobbyist license as we use 4-layers. We have also added Copper (http://www.copper-app.com) files that contain the complete BOM and a 3D view of the boards.
