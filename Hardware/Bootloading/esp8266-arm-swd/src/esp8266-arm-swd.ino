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
const int swd_clock_pin = 5;
const int swd_data_pin = 14;

// Edit these in the YOUR-WIFI-CONFIG tab
extern const char *host, *ssid, *password;

////////////////////////////////////////////////////////////////

#include "arm_debug.h"
#include "arm_kinetis_debug.h"
#include "arm_kinetis_reg.h"
#include "firmware_data.h"
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <HttpClient.h>

extern const uint8_t firmware[];
extern const uint32_t firmware_length;

extern const uint8_t firmware_slow[];
extern const uint32_t firmware_slow_length;

File firmware_file;

// WiFi information
const char WIFI_SSID[] = "Blue";
const char WIFI_PSK[] = "lmaPS2009";

// Remote site information
const char http_site[] = "www.appfruits.com";
const int http_port = 80;

// Pin definitions
const int LED_PIN = 0;

int mode = 0;

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

// Global variables
WiFiClient client;
HttpClient httpClient(client);

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

        Serial.println("Trying to unlock device");
        digitalWrite(reset_pin,LOW);
        target.flashMassErase();
        digitalWrite(reset_pin,HIGH);
        Serial.println("Unlock successful. Powercycle device and try again");

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

    //ARMKinetisDebug::FlashProgrammer programmer(target,(uint32_t*)firmware,numSectors);
    //ARMKinetisDebug::FlashProgrammer programmer(target,fw_data,fw_sectorCount);
    ARMKinetisDebug::Flasher programmer(target);
    if (!programmer.installFirmware(&firmware_file))
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

    firmware_file.close();

    Serial.println("Resetting Chip");
    digitalWrite(reset_pin,LOW);
    delay(200);
    digitalWrite(reset_pin,HIGH);
}

void setup()
{
    pinMode(0,INPUT_PULLUP);
    delay(2000);
    Serial.begin(115200);
    Serial.println("\n\n~ Starting up ~\n");

    SPIFFS.begin();

    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
        Serial.print(dir.fileName());
        File f = dir.openFile("r");
        Serial.println(f.size());
    }

    firmware_file = SPIFFS.open("/firmware.bin","r");
    Serial.print("Found firmware, size: ");
    Serial.println(firmware_file.size());
    firmware_file.close();

    //Serial.println("Connecting to WiFi");
    //connectWiFi();
}

// Attempt to connect to WiFi
void connectWiFi() {

    byte led_status = 0;

    // Set WiFi mode to station (client)
    WiFi.mode(WIFI_STA);

    // Initiate connection with SSID and PSK
    WiFi.begin(WIFI_SSID, WIFI_PSK);

    // Blink LED while we wait for WiFi connection
    while ( WiFi.status() != WL_CONNECTED ) {
        digitalWrite(LED_PIN, led_status);
        led_status ^= 0x01;
        delay(100);
    }

    // Turn LED on when we are connected
    digitalWrite(LED_PIN, HIGH);
}

void loop()
{
    int err =0;

    //server.handleClient();
    //delay(1);
    delay(10);

    //Idle - Wait for button press
    if (mode == 0)
    {
        if (digitalRead(0) == LOW)
        {
            delay(2000);
            if (digitalRead(0) == LOW)
            {
                mode = 2;
            }
        }
    }

    //Start Download
    if (mode == 1)
    {
        Serial.println("Starting firmware update, downloading file");

        if (httpClient.get("www.appfruits.com",80,"/files/Blink.bin") != 0)
        {
            Serial.println("Failed to request firmware file");
            mode = 0;
        }
        else
        {
            err = httpClient.responseStatusCode();
            if (err >= 0)
            {
                Serial.print("Got status code: ");
                Serial.println(err);

                // Usually you'd check that the response code is 200 or a
                // similar "success" code (200-299) before carrying on,
                // but we'll print out whatever response we get

                err = httpClient.skipResponseHeaders();
                if (err >= 0)
                {
                    int bodyLen = httpClient.contentLength();
                    Serial.print("Content length is: ");
                    Serial.println(bodyLen);
                    Serial.println();

                    firmware_file = SPIFFS.open("/firmware.bin","w");
                    if (!firmware_file)
                    {
                        Serial.println("Failed to open firmware file for writing");
                        mode = 0;
                        return;
                    }

                    // Now we've got to the body, so we can print it out
                    unsigned long timeoutStart = millis();
                    char c;

                    while ( (httpClient.connected() || httpClient.available()) &&
                    ((millis() - timeoutStart) < kNetworkTimeout) )
                    {
                        ESP.wdtFeed();

                        if (httpClient.available())
                        {
                            c = httpClient.read();
                            // Print out this character

                            firmware_file.write(c);

                            bodyLen--;
                            // We read something, reset the timeout counter
                            timeoutStart = millis();
                        }
                        else
                        {
                            // We haven't got any data, so let's pause to allow some to
                            // arrive
                            delay(kNetworkDelay);
                        }
                    }

                    firmware_file.close();
                    Serial.println("Downloaded file");
                    mode = 2;
                }
                else
                {
                    Serial.print("Failed to skip response headers: ");
                    Serial.println(err);
                    mode = 0;
                    return;
                }
            }
        }
    }

    //Update Firmware
    if (mode == 2)
    {
        firmware_file = SPIFFS.open("/firmware.bin","r");
        if (!firmware_file)
        {
            Serial.println("Failed to open firmware file for reading");
            mode = 0;
            return;
        }

        //Resetting Teensy
        Serial.println("Reseting Teensy - the hard way");
        pinMode(reset_pin,OUTPUT);
        digitalWrite(reset_pin,LOW);
        delay(500);
        digitalWrite(reset_pin,HIGH);

        Serial.println("Resetting Teensy done");
        delay(200);

        Serial.println("Flashing MK20");
        ESP.wdtDisable();
        flash();
        ESP.wdtEnable(10000);
        Serial.println("Flashing MK20 complete");

        mode = 0;
    }
}


void setup2() {

    // Set up serial console to read web page
    Serial.begin(115200);
    delay(1000);
    Serial.print("Thing GET Example");

    // Set up LED for debugging
    pinMode(LED_PIN, OUTPUT);

    Serial.println("Connecting WiFi");
    // Connect to WiFi
    connectWiFi();

    SPIFFS.begin();

    Serial.println("Downloading firmware");
    // Attempt to connect to website
    if ( !getPage() ) {
        Serial.println("GET request failed");
    }
}

void loop2() {

    // If there are incoming bytes, print them
    if ( client.available() ) {
        char c = client.read();
        //Serial.print(c);
        firmware_file.write(c);
        Serial.print(c);
    }

    // If the server has disconnected, stop the client and WiFi
    if ( !client.connected() ) {
        Serial.println("Download complete");

        // Close socket and wait for disconnect from WiFi
        client.stop();
        Serial.print("Finished downloading file, size: ");
        Serial.println(firmware_file.size());

        firmware_file.close();
        if ( WiFi.status() != WL_DISCONNECTED ) {
            WiFi.disconnect();
        }

        // Turn off LED
        digitalWrite(LED_PIN, LOW);

        // Do nothing
        Serial.println("Finished Thing GET test");
        while(true){
            delay(1000);
        }
    }
    else
    {
        digitalWrite(LED_PIN, HIGH);
    }
}



// Perform an HTTP GET request to a remote page
bool getPage() {

    Serial.println("Connecting website");
    // Attempt to make a connection to the remote server
    if ( !client.connect(http_site, http_port) ) {
        Serial.println("Failed to connect to site");
        return false;
    }

    Serial.println("Opening firmware file");
    firmware_file = SPIFFS.open("/firmware.bin","w");
    if (!firmware_file)
    {
        Serial.println("Failed to open file firmware.bin");
        return false;
    }

    Serial.println("Initiating download request");
    // Make an HTTP GET request
    client.println("GET /files/Blink.bin HTTP/1.1");
    client.print("Host: ");
    client.println(http_site);
    client.println("Connection: close");
    client.println();

    return true;
}
