//
// Created by Phillip Schuster on 30.01.16.
//

#include "ChoosePrintSceneController.h"

#include "Bitmaps.h"
#include "LabelButton.h"
#include "ModelView.h"
#include "MainSceneController.h"

ChoosePrintSceneController::ChoosePrintSceneController():
        SceneController::SceneController()
{
    ModelView* modelView = new ModelView(Rect(0,0,319,239));
    modelView->setJobName("Simple Metal Feet");
    modelView->setBitmap(feet);
    addView(modelView);

    modelView = new ModelView(Rect(320,0,319,239));
    modelView->setJobName("LCD Case 2");
    modelView->setBitmap(feet);
    addView(modelView);

    modelView = new ModelView(Rect(640,0,319,239));
    modelView->setJobName("Something useless");
    modelView->setBitmap(feet);
    addView(modelView);

    modelView = new ModelView(Rect(960,0,319,239));
    modelView->setJobName("Something useful");
    modelView->setBitmap(feet);
    addView(modelView);
}


void ChoosePrintSceneController::handleTouchUp(TS_Point &point)
{
    MainSceneController* mainSceneController = new MainSceneController();
    Application.pushScene(mainSceneController);
}

ChoosePrintSceneController::~ChoosePrintSceneController()
{

}


uint16_t ChoosePrintSceneController::getBackgroundColor()
{
    return Application.getTheme()->getBackgroundColor();
}

void ChoosePrintSceneController::display()
{

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

void ChoosePrintSceneController::setup()
{
    SceneController::setup();
}

#pragma mark ButtonDelegate Implementation

void ChoosePrintSceneController::buttonPressed(void *button)
{
    LOG("ChoosePrintSceneController::buttonPressed");

}
