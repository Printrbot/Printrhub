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

const int reset_pin = 13;
const int swd_clock_pin = 4;
const int swd_data_pin = 14;

// Edit these in the YOUR-WIFI-CONFIG tab
extern const char *host, *ssid, *password;

////////////////////////////////////////////////////////////////

#include "arm_debug.h"
#include "arm_kinetis_debug.h"
#include "arm_kinetis_reg.h"
#include "firmware_data.h"

extern const uint8_t firmware[];
extern const uint32_t firmware_length;

extern const uint8_t firmware_slow[];
extern const uint32_t firmware_slow_length;

// Turn the log level back up for debugging; but by default, we have it
// completely off so that even failures happen quickly, to keep the web app responsive.
ARMKinetisDebug target(swd_clock_pin, swd_data_pin, ARMDebug::LOG_NORMAL);

void flash()
{
    uint32_t idcode;
    if (target.begin() && target.getIDCODE(idcode)) {
        char result[128];

        // Note the room left in the API for future platforms detected,
        // even though it requires refactoring a bit.

        Serial.print("Debug Port Detected: ");
        Serial.println(idcode,HEX);
        //Serial.println("Target: " + target.detect());
    }

    delay(200);

    if (!target.detect())
    {
      Serial.println("Detect failed");
      return;
    }

/*    Serial.println("Erasing chip");
    if (!target.eraseEverything())
    {
      Serial.println("Failed to erase chip");
    }
    else
    {
      Serial.println("Flash erased");
    }*/

    delay(200);

    if (!target.reset())
    {
      Serial.println("Reset failed");
      return;
    }
    else
    {
      Serial.println("Device reset complete");
    }

    delay(200);

    if (!target.debugHalt())
    {
      Serial.println("Debug Halt failed");
    }
    else
    {
      Serial.println("Debug halt complete");
    }

    delay(200);

    if (!target.initK20())
    {
      Serial.println("K20 Init failed");
    }
    else
    {
      Serial.println("K20 initialized");
    }

    delay(200);

    Serial.println("Done");
    //return;

    int numSectors = firmware_length/1024;
    if (firmware_length % 1024 != 0) numSectors++;
    Serial.print("Num sectors: ");
    Serial.println(numSectors);

    //delay(3000);

    ARMKinetisDebug::FlashProgrammer programmer(target,(uint32_t*)firmware,numSectors);
    //ARMKinetisDebug::FlashProgrammer programmer(target,fw_data,fw_sectorCount);
    if (!programmer.installFirmware(firmware_slow,firmware_slow_length))
    {
      Serial.println("Failed to flash");
    }
/*    while (!programmer.isComplete())
    {
      if (!programmer.next())
      {
        Serial.println("Failed to flash");
        break;
      }
    }*/



    Serial.println("Resetting Chip");
    digitalWrite(reset_pin,LOW);
    delay(200);
    digitalWrite(reset_pin,HIGH);
}
/*
void setup()
{
  pinMode(0,OUTPUT);
  digitalWrite(0,HIGH);
    delay(2000);
    Serial.begin(115200);
    Serial.println("\n\n~ Starting up ~\n");

    //Resetting Teensy
    Serial.println("Reseting Teensy - the hard way");
    pinMode(reset_pin,OUTPUT);
    digitalWrite(reset_pin,LOW);
    delay(5000);
    digitalWrite(reset_pin,HIGH);

    Serial.println("Resetting Teensy done");
    delay(200);

    flash();
    return;
}

void loop()
{
    //server.handleClient();
    //delay(1);
    delay(10);
}
*/

void setup() {
  pinMode(0, OUTPUT);
  pinMode(reset_pin,OUTPUT);
}

void loop() {
  digitalWrite(0, HIGH);
  delay(500);
  digitalWrite(0,LOW);
  delay(100);
  digitalWrite(reset_pin,LOW);
  delay(1000);
}
