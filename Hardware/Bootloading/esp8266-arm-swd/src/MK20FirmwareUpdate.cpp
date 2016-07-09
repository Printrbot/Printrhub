//
// Created by Phillip Schuster on 24.06.16.
//

#include "MK20FirmwareUpdate.h"
#include "Idle.h"

const int reset_pin = 13;
const int swd_clock_pin = 5;
const int swd_data_pin = 14;

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

MK20FirmwareUpdate::MK20FirmwareUpdate():
Mode(),
target(swd_clock_pin, swd_data_pin, ARMDebug::LOG_NORMAL),
httpClient(client),
mode(StateDownloadFirmware)
{

}

MK20FirmwareUpdate::~MK20FirmwareUpdate()
{

}

String MK20FirmwareUpdate::getName()
{
    return "MK20FirmwareUpdate";
}

void MK20FirmwareUpdate::onWillStart()
{

}

void MK20FirmwareUpdate::onWillEnd()
{

}

void MK20FirmwareUpdate::flash()
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

    //Dump Memory
    target.dumpSector(0);
    target.dumpSector(1024);
    target.dumpSector(2048);

    Serial.println("Resetting Chip");
    digitalWrite(reset_pin,LOW);
    delay(200);
    digitalWrite(reset_pin,HIGH);
}

void MK20FirmwareUpdate::loop()
{
    int err = 0;

    //server.handleClient();
    //delay(1);
    delay(10);

    //Start Download
    if (mode == StateDownloadFirmware)
    {
        Serial.println("Starting firmware update, downloading file");

        if (httpClient.get("www.appfruits.com", 80, "/files/firmware.bin") != 0)
        {
            Serial.println("Failed to request firmware file");
            mode = StateError;
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

                    firmware_file = SPIFFS.open("/firmware.bin", "w");
                    if (!firmware_file)
                    {
                        Serial.println("Failed to open firmware file for writing");
                        mode = StateError;
                        return;
                    }

                    // Now we've got to the body, so we can print it out
                    unsigned long timeoutStart = millis();
                    char c;

                    while ((httpClient.connected() || httpClient.available()) &&
                           ((millis() - timeoutStart) < kNetworkTimeout))
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
                    mode = StateFlashFirmware;
                }
                else
                {
                    Serial.print("Failed to skip response headers: ");
                    Serial.println(err);
                    mode = StateError;
                    return;
                }
            }
        }
    }

    //Update Firmware
    if (mode == StateFlashFirmware)
    {
        firmware_file = SPIFFS.open("/firmware.bin", "r");
        if (!firmware_file)
        {
            Serial.println("Failed to open firmware file for reading");
            mode = StateError;
            return;
        }

        //Resetting Teensy
        Serial.println("Reseting Teensy - the hard way");
        pinMode(reset_pin, OUTPUT);
        digitalWrite(reset_pin, LOW);
        delay(500);
        digitalWrite(reset_pin, HIGH);

        Serial.println("Resetting Teensy done");
        delay(200);

        Serial.println("Flashing MK20");
        ESP.wdtDisable();
        flash();
        ESP.wdtEnable(10000);
        Serial.println("Flashing MK20 complete");

        mode = StateSuccess;
    }

    if (mode == StateError)
    {
        Mode* mode = new Idle();
        Application.pushMode(mode);
        return;
    }

    if (mode == StateSuccess)
    {
        Mode* mode = new Idle();
        Application.pushMode(mode);
        return;
    }
}