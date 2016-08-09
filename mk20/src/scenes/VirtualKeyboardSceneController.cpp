//
// Created by Phillip Schuster on 31.01.16.
//

#include "VirtualKeyboardSceneController.h"

//
// Created by Phillip Schuster on 20.01.16.
//

#include "MainSceneController.h"
#include "Bitmaps.h"
#include "framework/views/LabelButton.h"
#include "framework/core/Application.h"
//#include "ChoosePrintSceneController.h"
#include "WiFiSetupSceneController.h"
#include "font_AwesomeF080.h"
#include "font_AwesomeF000.h"
#include "framework/views/Button.h"

const char Keyboards[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','.',',',';','-','_','@',
                          'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','.',',',';','-','_','@',
                          '0','1','2','3','4','5','6','7','8','9','^','°','!','"','§','$','%','&','/','(',')','=','?','´','`','+','-','#','\'',':','<','>'};

VirtualKeyboardSceneController::VirtualKeyboardSceneController():
        SceneController::SceneController()
{
    //Setup Keyboard

    _currentKeyboard = 0;

    uint8_t i = 0;
    for (int y=1;y<5;y++)
    {
        for (int x=0;x<8;x++)
        {
            Rect frame = Rect(x*40,y*40,39,39);

            LabelButton* button = new LabelButton(String(Keyboards[_currentKeyboard*32+i]),frame);
            button->setName(String(Keyboards[_currentKeyboard*32+i]));
            button->setDelegate(this);
            addView(button);

            i++;
        }
    }

    LabelButton* button = new LabelButton(String((char)88),Rect(0,201,59,38));
    button->setName("Shift");
    button->setDelegate(this);
    button->setFont(&AwesomeF080_20);
    button->setButtonType(ButtonType::Toggle);
    button->setButtonState(ButtonState::On);
    addView(button);
    _shiftButton = button;

    button = new LabelButton("123",Rect(60,201,59,38));
    button->setBorderWidth(0);
    button->setName("123");
    button->setDelegate(this);
    button->setButtonType(ButtonType::Toggle);
    addView(button);

    button = new LabelButton("Space",Rect(121,201,137,38));
    button->setBorderWidth(0);
    button->setName(" ");
    button->setDelegate(this);
    button->setButtonType(ButtonType::Push);
    addView(button);

    button = new LabelButton(String((char)12),Rect(260,201,59,38));
    button->setTextColor(ILI9341_WHITE);
    button->setAlternateTextColor(ILI9341_WHITE);
    button->setBorderWidth(0);
    button->setName("Enter");
    button->setDelegate(this);
    button->setFont(&AwesomeF000_20);
    button->setAlternateFont(&AwesomeF000_20);
    button->setButtonType(ButtonType::Push);
    addView(button);

    _textField = new LabelView("",Rect(0,0,278,38));
    _textField->setFont(&PTSansNarrow_18);
    _textField->setVerticalTextAlign(TEXTALIGN_CENTERED);
    _textField->setTextAlign(TEXTALIGN_LEFT);
    _textField->setText(_text);
    //_textField->setTextColor(Application.getTheme()->getSecondaryColor2());
    _textField->setTextColor(ILI9341_WHITE);
    addView(_textField);

    button = new LabelButton(String((char)89),Rect(280,0,39,38));
    button->setBorderWidth(0);
    button->setName("Backspace");
    button->setDelegate(this);
    button->setButtonType(ButtonType::Push);
    button->setFont(&AwesomeF080_20);
    button->setAlternateFont(&AwesomeF080_20);
    addView(button);
}

VirtualKeyboardSceneController::~VirtualKeyboardSceneController()
{

}

uint16_t VirtualKeyboardSceneController::getBackgroundColor()
{
    return Application.getTheme()->getColor(SpacerColor);
}

String VirtualKeyboardSceneController::getName()
{
    return "VirtualKeyboardSceneController";
}

void VirtualKeyboardSceneController::updateKeyboard()
{
    for (int i=0;i<32;i++)
    {
        LabelButton* keyButton = (LabelButton*)getViews()->at(i);
        keyButton->setText(String(Keyboards[_currentKeyboard*32+i]));
        keyButton->setName(String(Keyboards[_currentKeyboard*32+i]));
        keyButton->setNeedsDisplay();
    }
}

#pragma mark ButtonDelegate Implementation

void VirtualKeyboardSceneController::buttonPressed(void *button)
{
    LOG("VirtualKeyboardSceneController::buttonPressed");

    LabelButton* labelButton = (LabelButton*)button;
    if (labelButton->getName() == "Shift")
    {
        if (labelButton->getButtonState() == ButtonState::Off)
        {
            _currentKeyboard = 1;
        }
        else
        {
            _currentKeyboard = 0;
        }

        updateKeyboard();
        return;
    }
    else if (labelButton->getName() == "123")
    {
        if (labelButton->getButtonState() == ButtonState::Off)
        {
            if (_shiftButton->getButtonState() == ButtonState::Off)
            {
                _currentKeyboard = 1;
            }
            else
            {
                _currentKeyboard = 0;
            }

            labelButton->setText("123");
        }
        else
        {
            _currentKeyboard = 2;

            if (_shiftButton->getButtonState() == ButtonState::Off)
            {
                labelButton->setText("abc");
            }
            else
            {
                labelButton->setText("ABC");
            }
        }

        updateKeyboard();
        return;
    }
    else if (labelButton->getName() == "Enter")
    {
        MainSceneController* mainSceneController = new MainSceneController();
        Application.pushScene(mainSceneController);
        return;
    }
    else if (labelButton->getName() == "Backspace")
    {
        if (_text != "")
        {
            _text = _text.substring(0,_text.length()-1);
        }
    }
    else
    {
        _text = _text + labelButton->getName();
    }

    _textField->setText(_text);
    _textField->setNeedsDisplay();
}
