# esp8266-arm-swd

Proof-of-concept ESP8266 web server and ARM Serial Wire Debug client!

This directory is an Arduino sketch.

The Serial Wire Debug client here is a slightly adapted version of the one I originally wrote for Fadecandy's factory test infrastructure. It has optional extensions for the Freescale Kinetis microcontrollers, but the lower-level SWD interface should be compatible with any ARM microcontroller.

Installing
----------

Software you'll need:

* The latest [Arduino IDE](http://www.arduino.cc/en/Main/Software)
* Install the [ESP8266 Arduino core](https://github.com/esp8266/Arduino)
* And the [filesystem plugin](https://github.com/esp8266/arduino-esp8266fs-plugin)

Now open `esp8266-arm-swd.ino` in the IDE:

1. In the Tools menu, select your ESP8266 board and serial port info
2. In the sketch, put your Wifi network info in the `YOUR-WIFI-CONFIG` tab
3. Sketch -> Upload
4. Tools -> ESP8266 Sketch Data Upload

Okay, now your Internet Thing is ready!

Usage
-----

Default URL: [http://esp8266-swd.local/](http://esp8266-swd.local/)

* Visit the web page, edit all the hexes
* Use the HTML5 components to make your own tools
* Send pull requests

Hookups:

| Signal      | SWD Port | SWD over JTAG | ESP8266 pin | Equiv. NodeMCU pin |
| ----------- | -------- | ------------- | ----------- | ------------------ |
| 3.3v Power  | Pin 1    |               |             |                    |
| Data In/Out | Pin 2    | TMS           | GPIO2       | D4                 |
| Ground      | Pin 3    |               |             |                    |
| Clock       | Pin 4    | TCLK          | GPIO0       | D3                 |

Development Proxy
-----------------

It's slow to update the filesystem, so for development work it's convenient to use a proxy to merge your local static files with the sketch's live API.

A simple tool using Node is included:

```shell
npm install
node webdev-proxy.js
```
