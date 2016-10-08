//
// Created by Phillip Schuster on 05.10.16.
//

#ifndef MK20_CONFIRMFIRMWAREUPDATESCENE_H
#define MK20_CONFIRMFIRMWAREUPDATESCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/LabelButton.h"

class ConfirmFirmwareUpdateScene: public SidebarSceneController {
public:

    ConfirmFirmwareUpdateScene();
    virtual ~ConfirmFirmwareUpdateScene();

    virtual void handleTouchMoved(TS_Point point, TS_Point oldPoint) override;
    virtual void animationFinished(Animation *animation) override;
    virtual void onSidebarButtonTouchUp() override;

    virtual uint16_t getBackgroundColor() override;
    virtual UIBitmap * getSidebarBitmap() override;
    virtual UIBitmap * getSidebarIcon() override;
    virtual bool isModal() override;

private:
    virtual void onWillAppear() override;
    String getName() override;
    virtual void buttonPressed(void *button) override;

protected:
    LabelButton* _upgradeButton;
};

#endif //MK20_CONFIRMFIRMWAREUPDATESCENE_H
