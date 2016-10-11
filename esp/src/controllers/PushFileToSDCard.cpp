//
// Created by Phillip Schuster on 11.10.16.
//

#include "PushFileToSDCard.h"
#include "../event_logger.h"

PushFileToSDCard::PushFileToSDCard(String localFilePath, String targetFilePath, bool showUI):
Mode(),
_localFilePath(localFilePath),
_targetFilePath(targetFilePath),
_waitForResponse(false),
_fileOpen(false)
{
    EventLogger::log("Request SD file write with path: %s",_targetFilePath.c_str());

    if (!SPIFFS.exists(localFilePath)) {
        EventLogger::log("Local file does not exist: %s",_localFilePath.c_str());
        exitWithError(DownloadError::LocalFileNotFound);
    } else {
        _localFile = SPIFFS.open(localFilePath,"r");
        if (!_localFile) {
            EventLogger::log("Local file could not be opened for read: %s",_localFilePath.c_str());
            exitWithError(DownloadError::LocalFileOpenForReadFailed)
        } else {
            _bytesLeft = _localFile.size();
            EventLogger::log("File opened successfully: %s, Bytes left to send: %d",_localFilePath.c_str(),_bytesLeft);

            //Request MK20 to store a file on SD card
            _waitForResponse = true;
            Application.getMK20Stack()->openSDFileForWrite(_targetFilePath,_bytesLeft, showUI);
        }
    }
}

PushFileToSDCard::~PushFileToSDCard()
{
    _localFile.close();
}

void PushFileToSDCard::loop()
{
    //Just send data once the file is open
    if (!_fileOpen) {
        return;
    }

    //We wait for a response of MK20
    if (_waitForResponse) {
        return;
    }

    uint8_t buffer[128];
    int numReadBytes = _localFile.read(buffer,_bytesLeft > 128 ? 128 : _bytesLeft);
    _bytesLeft -= numReadBytes;

    EventLogger::log("Sending %d bytes to SD card, bytes left: %d",numReadBytes, _bytesLeft);

    //Send bytes
    _waitForResponse = true;
    Application.getMK20Stack()->sendSDFileData(buffer,numReadBytes);

    if (_bytesLeft <= 0) {
        EventLogger::log("Sending file complete");

        //File is completely transferred
        Application.getMK20Stack()->closeSDFile();

        //Close local file
        _localFile.close();

        //Exit this mode
        exit();
    }
}

bool PushFileToSDCard::handlesTask(TaskID taskID)
{
    if (taskID == TaskID::FileOpenForWrite) {
        return true;
    } else if (taskID == TaskID::FileSaveData) {
        return true;
    }

    return false;
}

bool PushFileToSDCard::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success)
{
    if (header.getCurrentTask() == TaskID::FileOpenForWrite) {
        if (header.commType == ResponseSuccess) {
            _waitForResponse = false;
            _fileOpen = true;
        } else if (header.commType == ResponseFailed) {
            _waitForResponse = false;
            _fileOpen = false;
            exitWithError(DownloadError::TargetFileOpenForWriteFailed);
        }

    } else if (header.getCurrentTask() == TaskID::FileSaveData) {
        _waitForResponse = false;
    }
}
