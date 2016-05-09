//
// Created by Phillip Schuster on 30.01.16.
//

#include "ChoosePrintSceneController.h"

#include "Bitmaps.h"
#include "framework/views/LabelButton.h"
#include "ModelView.h"
#include "MainSceneController.h"
#include "drivers/SD/SD.h"

ChoosePrintSceneController::ChoosePrintSceneController():
        SceneController::SceneController()
{
    ModelView* modelView = new ModelView(Rect(0,0,269,239));
    modelView->setJobName("Simple Metal Feet");
    modelView->setImageFileName("feet.dat");
    addView(modelView);

    modelView = new ModelView(Rect(270,0,269,239));
    modelView->setJobName("LCD Case");
    modelView->setImageFileName("lcd.dat");
    addView(modelView);

    modelView = new ModelView(Rect(540,0,269,239));
    modelView->setJobName("MacBook Pro Holder");
    modelView->setImageFileName("macbook.dat");
    addView(modelView);

    modelView = new ModelView(Rect(810,0,269,239));
    modelView->setJobName("Cable Hook");
    modelView->setImageFileName("hook.dat");
    addView(modelView);

    /*View* menuView = new View(0,0,40,240);
    RectangleLayer* menuRect = new RectangleLayer(Rect(0,0,50,240));
    menuRect->setBackgroundColor(ILI9341_OLIVE);
    menuView->addLayer(menuRect);
    addView(menuView);*/
}


void ChoosePrintSceneController::handleTouchUp(TS_Point &point)
{
//    MainSceneController* mainSceneController = new MainSceneController();
//    Application.pushScene(mainSceneController);

    if (point.x < 50)
    {
        MainSceneController* scene = new MainSceneController();
        Application.pushScene(scene);
    }
}

ChoosePrintSceneController::~ChoosePrintSceneController()
{

}


uint16_t ChoosePrintSceneController::getBackgroundColor()
{
    return Application.getTheme()->getColor(BackgroundColor);
}

String ChoosePrintSceneController::getName()
{
    return "ChoosePrintSceneController";
}

void ChoosePrintSceneController::loop()
{
    SceneController::loop();

/*    _printButton->setFrame(Rect(23,90+_offset,80,80));
    _filamentButton->setFrame(Rect(120,90+(_offset/2),80,80));
    _settingsButton->setFrame(Rect(215,90+(_offset/4),80,80));
    _offset += _velocity;

    if (_offset > 80 || _offset < -80)
    {
        _velocity = -_velocity;
    }*/

    /*   if (_transition)
       {
           _printButton->setFrame(Rect(23+_offset,90+_offset,80,80));
           _filamentButton->setFrame(Rect(120,90+(_offset/2),80,80));
           _settingsButton->setFrame(Rect(215,90+(_offset/4),80,80));
           _offset += _velocity;

           if (_offset > 200 || _offset < -200)
           {
               _velocity = -_velocity;
           }

           return;
       }

       if (Touch.touched())
       {
           //Y = X;

           TS_Point point = Touch.getPoint();
           swap(point.x,point.y);

           point.y = 240-point.y;


           //if (_printButton->getFrame().containsPoint(point.x,point.y))
           {
               //_printButton->setFrame(Rect(23,93,80,80));
           }
   //        else
   //        {
   //            _printButton->setFrame(Rect(23,90,80,80));
   //        }

           _transition = true;
       }
       else
       {
          // _printButton->setFrame(Rect(23,89,80,80));
       }*/
}


void ChoosePrintSceneController::onWillAppear()
{
    SceneController::onWillAppear();

    Display.setScrollInsets(50,0);
}

#pragma mark ButtonDelegate Implementation

void ChoosePrintSceneController::buttonPressed(void *button)
{
    LOG("ChoosePrintSceneController::buttonPressed");

}
