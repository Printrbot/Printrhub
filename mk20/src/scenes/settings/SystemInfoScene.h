//
// Created by Phillip Schuster on 13.10.16.
//

#ifndef MK20_SYSTEMINFO_H
#define MK20_SYSTEMINFO_H


#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelView.h"

class SystemInfoScene: public SidebarSceneController {
public:

    SystemInfoScene();
    virtual ~SystemInfoScene();
    virtual void onSidebarButtonTouchUp() override;

    virtual uint16_t getBackgroundColor() override;
    virtual UIBitmap * getSidebarBitmap() override;
    virtual UIBitmap * getSidebarIcon() override;

private:
    virtual void onWillAppear() override;
    String getName() override;
    virtual void buttonPressed(void *button) override;
    virtual void loop() override;
    virtual bool isModal() override;

    virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);
    virtual bool handlesTask(TaskID taskID);

    void configureLabelView(LabelView* labelView);
    void queryData();

private:
    LabelView* _printerNameLabel;
    LabelView* _printerName;
    LabelView* _ipaddressLabel;
    LabelView* _ipaddress;
    LabelView* _ssidLabel;
    LabelView* _ssid;
    LabelView* _serialNumberLabel;
    LabelView* _serialNumber;
    LabelView* _firmwareVersionLabel;
    LabelView* _firmwareVersion;
    LabelView* _networkModeLabel;
    LabelView* _networkMode;
    SystemInfo _systemInfo;
    bool _dataReceived;
    unsigned long _lastPing;
};

#endif //MK20_SYSTEMINFO_H
