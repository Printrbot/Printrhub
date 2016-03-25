# Flashing Teensy MCU

Having modified the ARMDebug/ARMDebugKinetis library from scanlime (see below) it’s possible to connect, reset, halt and erase the flash from the MCU. The modification involved has been adding small delays between clock HIGH and LOW as even J-Link connections with more than 1000 KHz did not work reliable. 

But flashing does not work. It breaks after sector erase with a flash access error. The datasheet does not further specify this error. There are numerous possibilities causing that. From protection flags, which shouldn’t be there. 

The way of doing flash programming is by setting the FlexRAM to RAM mode (instead of EEPROM), writing 1024 bytes to it and then triggering the flash program section command which writes the number of words to the address given. This command fails, or it looks like as it would fail. 

I tried flipping the registers (i.e. FCCOB5 with FCCOB4) which worked, i.e. the program ran through, but verification failed. The reason’s been ease. By flipping both registers only 1 word has been written. So it looks like numLWords is the error. It’s calculated by 1024/4 which should work according numerous posts in the freescale forums.

But perhaps switching to 32bit before does not work and the MCU expects 1024 instead of 256 for numLWords?

It’s also possible that the command sequence does not work. The fadecandy code does not precisely match the sequence. 

Analyzing the SWD protocol of the Teensy and the flash programmer does not show that these use this method as I cannot see any FCCOB register write command.

K20 should feature writing longwords directly to the flash. Perhaps this is what Teeny and J-Link do, but it’s not documented in the datasheet that this even exists.

## J-Link
SWD-Pinout: https://www.segger.com/interface-description.html
Unlocking and Erasing Flash: http://mcuoneclipse.com/2014/10/05/unlocking-and-erasing-flash-with-segger-j-link/
This web page has other interesting blog posts about J-Link, debugging and SWD

## McHck
This is a Teensy based board with custom bootloader.
https://github.com/mchck/mchck

## Word Alignment
Word alignment means, that the address can be divided by 4. 
https://www.cs.umd.edu/class/sum2003/cmsc311/Notes/Data/aligned.html

## SWD
This datasheet (other MCU) has a lot of interesting information about SWD and flash programming in general:
https://www.silabs.com/Support%20Documents/TechnicalDocs/AN0062.pdf

## Teensy Flash
This project shows how to write to the flash:
https://github.com/FrankBoesing/Arduino-Teensy3-Flash/blob/master/flashKinetis.cpp

## Fadecandy
Another Teensy based project without the Teensy bootloader. Has application code for Arduino based SWD debugging. This code works mostly fine, but programming the flash memory does not work:
https://github.com/scanlime/fadecandy

## uTasker
Has a serial flash bootloader in source. Didn’t try yet:
http://www.utasker.com/forum/index.php?topic=1721.msg6697#msg6697

## KDS
How to install and use Kinetis Design Studio:
http://mcuoneclipse.com/2015/07/23/how-to-use-kinetis-design-studio/
