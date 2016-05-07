//
// Created by Phillip Schuster on 31.01.16.
//

#ifndef TEENSYCMAKE_VIRTUALKEYBOARDSCENECONTROLLER_H
#define TEENSYCMAKE_VIRTUALKEYBOARDSCENECONTROLLER_H


#include "SceneController.h"
#include "BitmapButton.h"
#include "LabelView.h"
#include "LabelButton.h"

class VirtualKeyboardSceneController: public SceneController, ButtonDelegate
{
#pragma mark Constructor
public:
    VirtualKeyboardSceneController();
    virtual ~VirtualKeyboardSceneController();

#pragma mark Scene Controller
    String getName();
    virtual uint16_t getBackgroundColor() override;

#pragma mark Button Delegate
private:
    virtual void buttonPressed(void *button);

#pragma mark Member Functions
    void updateKeyboard();

#pragma mark Member Variables
public:
    String _text;
    LabelView* _textField;
    LabelButton* _shiftButton;
    uint8_t _currentKeyboard;
};

#endif //TEENSYCMAKE_VIRTUALKEYBOARDSCENECONTROLLER_H
