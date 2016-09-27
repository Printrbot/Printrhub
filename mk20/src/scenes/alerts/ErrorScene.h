//
// Created by Phillip Schuster on 27.09.16.
//

#ifndef MK20_ERRORSCENE_H
#define MK20_ERRORSCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelButton.h"

class ErrorScene: public SidebarSceneController {
public:

    ErrorScene(const String& errorMessage);
    virtual ~ErrorScene();

    virtual void onSidebarButtonTouchUp() override;

    virtual uint16_t getBackgroundColor() override;
    virtual UIBitmap * getSidebarBitmap() override;
    virtual UIBitmap * getSidebarIcon() override;

private:
    virtual void onWillAppear() override;
    String getName() override;
    virtual void buttonPressed(void *button) override;

protected:
    LabelButton* _okBtn;
    String _errorMessage;
};

#endif //MK20_ERRORSCENE_H
