////////////////////////////////////////////////////////////////
//
//    Proof of Concept ESP8266 Wifi + HTTP web interface
//     for remote control of an ARM microcontroller
//      via its Serial Wire Debug port.
//
//    Tested with the FRDM-KE04Z dev kit (KE04 Cortex M0+)
//    and the Fadecandy board (K20 Cortex M4)
//
////////////////////////////////////////////////////////////////

// Copyright (c) 2015 Micah Elizabeth Scott
// Released with an MIT-style license; see LICENSE file

// Default pins:
//    ESP-01          GPIO0 = swdclk, GPIO2 = swdio
//    NodeMCU devkit  D3 = swdclk, D4 = swdio
//
// And for reference:
//    SWD header      pin1 = 3.3v, pin2 = swdio, pin3 = gnd, pin4 = swdclk
//    SWD over JTAG   TCLK = swdclk, TMS = swdio

////////////////////////////////////////////////////////////////

#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <HttpClient.h>
#include "core/Application.h"
#include "WiFiSetup.h"
#include "MK20FirmwareUpdate.h"
#include "Idle.h"

void setup()
{
    Mode* initialMode = new Idle();
    Application.pushMode(initialMode);

    Application.setup();
}

void loop()
{
    Application.loop();
}