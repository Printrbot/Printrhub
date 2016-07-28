//
// Created by Phillip Schuster on 30.01.16.
//

#include "ChoosePrintSceneController.h"

#include "Bitmaps.h"
#include "framework/views/LabelButton.h"
#include "ModelView.h"
#include "MainSceneController.h"
#include "drivers/SD/SD.h"
#include "SettingsSceneController.h"
#include "ConfirmSceneController.h"
#include "PrintStatusSceneController.h"
#include "DownloadFileController.h"

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
    return imageOfWrenchIcon_24_24;
}

void ChoosePrintSceneController::onWillAppear()
{
    setScrollSnap(Display.getLayoutWidth(), SnapMode::Flick);

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

    _printButton = new LabelButton("PRINT",Rect(50,200,170,30));
    _printButton->setVisible(false);
    addView(_printButton);

    _printButton->log();

    SidebarSceneController::onWillAppear();
}


void ChoosePrintSceneController::onSidebarButtonTouchUp()
{
    SettingsSceneController * scene = new SettingsSceneController();
    Application.pushScene(scene);
}


void ChoosePrintSceneController::handleTouchMoved(TS_Point point, TS_Point oldPoint)
{
    if (_printButton != NULL)
    {
        _printButton->setVisible(false);

        //This would immediately remove the button but it's too slow
        //Display.invalidateRect(_printButton->getFrame());
    }

    SceneController::handleTouchMoved(point, oldPoint);
}


void ChoosePrintSceneController::animationFinished(Animation *animation)
{
    SceneController::animationFinished(animation);

    //We should have stopped at a defined slot index, use that to position the button
    int index = getPageIndex();
    float x = Display.getLayoutWidth() * index;

    _printButton->setFrame(Rect((x+50),200,170,30));
    _printButton->setVisible(true);
    _printButton->setDelegate(this);
}


#pragma mark ButtonDelegate Implementation

void ChoosePrintSceneController::buttonPressed(void *button)
{
    if (button == _printButton)
    {
        bool jobExists = false;

        if (jobExists)
        {
            LOG_VALUE("Printing Job-Nr",getPageIndex());
            PrintStatusSceneController * scene = new PrintStatusSceneController();
            Application.pushScene(scene);
        }
        else
        {
            LOG_VALUE("Need to download file",getPageIndex());
            DownloadFileController* scene = new DownloadFileController("www.appfruits.com",80,"/files/fs.png","fs.png");
            Application.pushScene(scene);
        }
    }

    SidebarSceneController::buttonPressed(button);
}
