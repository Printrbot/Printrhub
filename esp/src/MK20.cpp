//
// Created by Phillip Schuster on 04.10.16.
//

#include "MK20.h"
#include "application.h"
#include "arm_kinetis_debug.h"

MK20::MK20(Stream *port, CommStackDelegate *delegate):
CommStack(port,delegate)
{
    _numTries = 0;
    _isAlive = false;
    _timeout;
}

MK20::~MK20()
{

}

void MK20::reset()
{
    pinMode(MK20_RESET_PIN, OUTPUT);
    digitalWrite(MK20_RESET_PIN, LOW);
    delay(20);
    digitalWrite(MK20_RESET_PIN, HIGH);
    pinMode(MK20_RESET_PIN, INPUT);
}

bool MK20::writeFirmware(File &firmware_file)
{
    EventLogger::log("Reseting Teensy - the hard way");

    //Reset MK20
    reset();

    EventLogger::log("Resetting Teensy done");
    delay(200);

    EventLogger::log("Flashing");
    ESP.wdtDisable();

    // flash here
    ARMKinetisDebug target(MK20_SWD_CLK, MK20_SWD_IO, ARMDebug::LOG_NORMAL);
    uint32_t idcode;
    if (target.begin() && target.getIDCODE(idcode)) {
        char result[128];
        EventLogger::log("Debug Port Detected:");
    }
    delay(200);

    if (!target.detect()) {
        EventLogger::log("Detect failed:");
        return false;
    }
    delay(200);

    if (!target.reset()) {
        EventLogger::log("Reset failed:");
        EventLogger::log("Trying to unlock device:");
        digitalWrite(MK20_RESET_PIN,LOW);
        target.flashMassErase();
        digitalWrite(MK20_RESET_PIN,HIGH);
        EventLogger::log("Unlock successful. Powercycle device and try again");
        return false;
    } else {
        EventLogger::log("Device reset complete");
    }
    delay(200);

    if (!target.debugHalt()) {
        EventLogger::log("Debug Halt failed");
    } else {
        EventLogger::log("Debug halt complete");
    }
    delay(200);

    if (!target.initK20()) {
        EventLogger::log("MK20 Init faile");
    }
    else {
        EventLogger::log("MK20 initialized");
    }
    delay(200);

    ARMKinetisDebug::Flasher programmer(target);
    if (!programmer.installFirmware(&firmware_file)) {
        EventLogger::log("Failed to flash");
        return false;
    }

    firmware_file.close();
    target.dumpSector(0);
    target.dumpSector(1024);
    target.dumpSector(2048);

    EventLogger::log("Resetting Chip");
    reset();

    // end flash
    ESP.wdtEnable(10000);
    EventLogger::log("Flashing MK20 complete");

    //Start over with timeouts, etc.
    _timeout = 0;
    _numTries = 0;
    _isAlive = false;

    return true;
}

bool MK20::updateFirmware()
{
    File firmware_file = SPIFFS.open("/mk20.bin", "r");
    if (firmware_file) {
        //Flash MK20 with latest (default) firmware.
        return writeFirmware(firmware_file);
    }
    else {
        //TODO: Should not happen. We don't have the core firmware file, download it now?
        return false;
    }

    return false;
}

void MK20::showUpdateFirmwareNotification()
{
    if (Application.getFirmwareUpdateInfo() == NULL) return;

    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& updateInfo = jsonBuffer.createObject();
    updateInfo["version"] = "0.13";
    updateInfo["buildnr"] = Application.getFirmwareUpdateInfo()->buildnr;

    //Ask MK20 to show the notification to the user and we are finished here
    requestTask(TaskID::ShowFirmwareUpdateNotification,updateInfo);

    //Wait a bit until the screen has been updated
    delay(200);
}
