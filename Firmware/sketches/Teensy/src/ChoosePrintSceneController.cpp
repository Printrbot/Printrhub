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
        SidebarSceneController::SidebarSceneController()
{


    /*View* menuView = new View(0,0,40,240);
    RectangleLayer* menuRect = new RectangleLayer(Rect(0,0,50,240));
    menuRect->setBackgroundColor(ILI9341_OLIVE);
    menuView->addLayer(menuRect);
    addView(menuView);*/
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


String ChoosePrintSceneController::getSidebarTitle() const
{
    return String("PROJECTS");
}

const uint8_t *ChoosePrintSceneController::getSidebarIcon()
{
    return imageOfCancelIcon_24_24;
}

void ChoosePrintSceneController::onWillAppear()
{
    setScrollSnap(Display.getLayoutWidth());

    //As the model views are distributed as opaque, seamless tiles we don't need auto layout as we don't have spaces where
    //background shines through
    Display.disableAutoLayout();

    ModelView* modelView = new ModelView(Rect(0,0,270,240));
    modelView->setJobName("Simple Metal Feet");
    modelView->setImageFileName("feet.dat");
    modelView->setColor(ILI9341_CYAN);
    addView(modelView);

    modelView = new ModelView(Rect(270,0,270,240));
    modelView->setJobName("LCD Case");
    modelView->setImageFileName("lcd.dat");
    modelView->setColor(ILI9341_RED);
    addView(modelView);

    modelView = new ModelView(Rect(540,0,270,240));
    modelView->setJobName("MacBook Pro Holder");
    modelView->setImageFileName("macbook.dat");
    modelView->setColor(ILI9341_OLIVE);
    addView(modelView);

    modelView = new ModelView(Rect(810,0,270,240));
    modelView->setJobName("Cable Hook");
    modelView->setImageFileName("hook.dat");
    modelView->setColor(ILI9341_PURPLE);
    addView(modelView);

    SidebarSceneController::onWillAppear();
}


void ChoosePrintSceneController::onSidebarButtonTouchUp()
{
    SettingsSceneController * scene = new SettingsSceneController();
    Application.pushScene(scene);
}

#pragma mark ButtonDelegate Implementation

void ChoosePrintSceneController::buttonPressed(void *button)
{
    LOG("ChoosePrintSceneController::buttonPressed");

    SidebarSceneController::buttonPressed(button);
}
