//
// Created by Phillip Schuster on 04.10.16.
//

#ifndef ESP_MK20_H
#define ESP_MK20_H

#include "core/CommStack.h"
#include "config.h"
#include <FS.h>

class MK20 : public CommStack
{
public:
    MK20(Stream* port, CommStackDelegate* delegate);
    ~MK20();

    void reset();
    bool writeFirmware(File &file);
    bool updateFirmware(String localFilePath);
    void showUpdateFirmwareNotification();

    bool isTimedOut() { return !_isAlive && _timeout > 10; };
    void incrementTimeout(float deltaTime) { _timeout += deltaTime; };
    uint8_t getNumTries() { return _numTries; };
    void incrementNumTries() { _numTries++; _timeout = 0; };
    void setBuildNumber(int buildNumber) { _buildNumber = buildNumber; _isAlive = true; _timeout = 0; _numTries = 0; };
    bool isAlive() { return _isAlive; };
    bool needsUpdate() { return FIRMWARE_BUILDNR > _buildNumber; };
    bool openSDFileForWrite(String targetFilePath, size_t bytesToSend, bool showUI=false);
    bool sendSDFileData(uint8_t* data, size_t size);
    bool closeSDFile();

private:
    unsigned long _timeout;
    bool _isAlive;
    uint8_t _numTries;
    int _buildNumber;
};

#endif //ESP_MK20_H
