//
// Created by Phillip Schuster on 05.10.16.
//

#include "CheckForFirmwareUpdates.h"
#include "Idle.h"

CheckForFirmwareUpdates::CheckForFirmwareUpdates():
DownloadURL(FIRMWAREUPDATE_URL),
_content(NULL),
_pointer(0),
_size(0)
{}

CheckForFirmwareUpdates::~CheckForFirmwareUpdates()
{
    if (_content != NULL) {
        free(_content);
        _content = NULL;
    }
}

String CheckForFirmwareUpdates::getName()
{
    return "CheckForFirmwareUpdates";
}

bool CheckForFirmwareUpdates::onBeginDownload(uint32_t expectedSize)
{
    digitalWrite(COMMSTACK_INFO_MARKER_PIN, LOW);
    _size = expectedSize;
    _content = (uint8_t*)malloc(sizeof(uint8_t)*expectedSize);
    if (!_content) {
        return false;
    }

    _pointer = 0;

    return true;
}

bool CheckForFirmwareUpdates::onDataReceived(uint8_t *data, uint16_t size)
{
    memcpy(&_content[_pointer],data,size);
    _pointer += size;

    return true;
}

void CheckForFirmwareUpdates::onError(DownloadError errorCode)
{
    //TODO: Decide if and what to show the user in this case, today we just do nothing at all
    Idle* idle = new Idle();
    Application.pushMode(idle);
}

void CheckForFirmwareUpdates::onFinished()
{
    digitalWrite(COMMSTACK_INFO_MARKER_PIN, HIGH);
    if (_content != NULL) {
        StaticJsonBuffer<500> jsonBuffer;
        String jsonObject((const char*)_content);
        JsonObject& root = jsonBuffer.parseObject(jsonObject);

        if (root.success()) {
            //We have a newer firmware available, save update infos like urls in Application
            FirmwareUpdateInfo* info = new FirmwareUpdateInfo();
            int buildNumber = root["version"];
            const char* esp_url = root["esp_url"];
            const char* mk20_url = root["mk20_url"];
            const char* mk20_ui_url = root["mk20_ui_url"];

            info->buildnr = buildNumber;
            info->esp_url = String(esp_url);
            info->mk20_url = String(mk20_url);
            info->mk20_ui_url = String(mk20_ui_url);
            Application.setFirmwareUpdateInfo(info);
        }
    }

    //Whatever is, we get in idle state as now either MK20 runs the project scene or is showing an update notification to the user
    Idle* idle = new Idle();
    Application.pushMode(idle);
}
