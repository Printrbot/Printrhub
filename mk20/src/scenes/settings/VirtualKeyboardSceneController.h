//
// Created by Phillip Schuster on 31.01.16.
//

#ifndef TEENSYCMAKE_VIRTUALKEYBOARDSCENECONTROLLER_H
#define TEENSYCMAKE_VIRTUALKEYBOARDSCENECONTROLLER_H


#include "framework/core/SceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelView.h"
#include "framework/views/LabelButton.h"

class VirtualKeyboardHandler
{
public:
    virtual void onFinished(String textEntered) = 0;
    virtual void onCancelled() = 0;
};

class VirtualKeyboardSceneController: public SceneController, ButtonDelegate
{
#pragma mark Constructor
public:
    VirtualKeyboardSceneController(VirtualKeyboardHandler* handler, const char* name);
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
    LabelView* _nameField;
    LabelButton* _shiftButton;
    uint8_t _currentKeyboard;
    VirtualKeyboardHandler* _handler;
};

#endif //TEENSYCMAKE_VIRTUALKEYBOARDSCENECONTROLLER_H
