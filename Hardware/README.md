
# Modules and important components
Printrhub consists of a main MCU running most of the display and hub code, a 2.8 inch TFT display connected via SPI, a capacitive touch controller connected via I2C, a micro SD card slot connected via SPI to store G-Codes and meta-data about print jobs and an ESP8266 module connected via UART and SWD that flashes the main MCUs firmware (OTA updates), listens for external print jobs and downloads G-Codes which are sent via UART to the main MCU which stores them on the SD card.

We also need a powerful LDO to serve the modules with 3.3V and enough current to handle the required temperature range. 

## Display
It’s important that the display runs the ILI9341 driver chip as our code and display driver is optimized for it. We chose a 2.8 inch TFT display with 320x240 pixels. The display is connected via SPI. Attached to the display is a capacitive touch screen with FT6206 controller chip. 

http://www.buydisplay.com/default/serial-spi-2-8-tft-lcd-module-display-320x240-optional-touch-screen

## ESP8266
According to the specifications max typical consumption is 170mA. (Source: https://www.adafruit.com/images/product-files/2471/0A-ESP8266__Datasheet__EN_v4.3.pdf)

But I read somewhere it can peak up to 250mA (Source: https://www.adafruit.com/products/2471)

**Peak power consumption**:
We have a peak power consumption of 250mA. To be safe for different temperatures we perhaps we calculate with 300mA.

**Possible Module:**
https://www.adafruit.com/products/2491

**Schematics on how to use this module:**
https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/downloads

## MK20DX256VLH7
This is the main MCU where display code and hub software runs. It will be flashed in-circuit (OTA updates) with the ESP8266 by SWD.

**Peak power consumption**:
According to the datasheet the peak power consumption is 39mA with everything enabled and at 120°C. More typical will be a consumption of around 30mA. 

**Source:**
http://cache.freescale.com/files/32bit/doc/data_sheet/K20P64M72SF1.pdf

## LDO
The onboard regulator of MK20DX256VLH7 is only capable of serving 150mA external consumption, so we cannot use the internal regulator.

As we don’t have very hard low power constraints (we are running a 3D printer with heaters, so we don’t care about 1 or 2 mA) we will use a simple, powerful, cheap and efficient LDO, instead of a more complex switching regulator (introduces noise that must be handled with careful placement of power planes and a cap network, also (often) requires an inductor)

As the Printrboard only serves 300mA on it's 5V „line“ (the schematics of Rev F show the part LM2841) we cannot use that and we have to connect the Printrhub to the 12V, requiring an LDO with Vin >= 12V.

Power consumption of components on Printrhub:
* **ESP8266**: 300mA
* **MK20DX256VLH7**: 40mA
* **SD Card**: 100-200mA
* **Display**: 75mA backlight, 15mA logic

As we need 1A out at 3.3V the best option is to use a SOT-223-3 footprint as there are many parts available with this footprint and it should feature a nice heat transition due to large pins distributing heat to the copper planes.
Possible option:
**AP1117E33G-13** (Fixed 3.3V out, 18V in, 1A out)

## Micro SD card
As we do not expose this slot we don’t need any fancy, user friendly escape mechanisms driving costs. 

Preferred Part: **DM3D-SF** (cheap and simple)

