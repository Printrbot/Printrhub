set(TEENSY_USB_MODE SERIAL)

import_arduino_library(SPI)
import_arduino_library(WIRE)
import_arduino_library(Adafruit_FT6206)
import_arduino_library(ILI9341_t3)

# Create a target for the sketch in this directory.
add_sketch()
