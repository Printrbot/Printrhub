#include "ProjectsScene.h"

#include "Bitmaps.h"
#include "framework/views/LabelButton.h"
#include "ModelView.h"
#include "SD.h"
#include "../settings/SettingsSceneController.h"
#include "../print/PrintStatusSceneController.h"
#include "../DownloadFileController.h"
#include "../SidebarSceneController.h"

ProjectsScene::ProjectsScene(): SidebarSceneController::SidebarSceneController() {
}

ProjectsScene::~ProjectsScene() {
}


String ProjectsScene::getName() {
  return "ProjectsScene";
}

String ProjectsScene::getSidebarTitle() const {
  return String("PROJECTS");
}

const uint8_t *ProjectsScene::getSidebarIcon() {
  return imageOfWrenchIcon_24_24;
}

void ProjectsScene::onWillAppear() {
    setScrollSnap(Display.getLayoutWidth(), SnapMode::Flick);

    //As the model views are distributed as opaque, seamless tiles we don't need auto layout as we don't have spaces where
    //background shines through
    Display.disableAutoLayout();

    // loop through the projects directory
    const char * pdirname = "/projects";
    File pdir = SD.open(pdirname);
    pdir.rewindDirectory();

    ModelView* modelView;


    for (int cnt = 0; true; ++cnt) {
      File entry = pdir.openNextFile();

      if (!entry) break;

      modelView = new ModelView(Rect(270 * cnt,0,270,240));
      modelView->setJobName(entry.name()); // this will come from file itself

      String img = pdirname;
      img += "/";
      img += entry.name();

      modelView->setImageFileName(img);
      addView(modelView);
      entry.close();
   }

   _printButton = new LabelButton("PRINT",Rect(50,200,170,30));
   _printButton->setVisible(false);
   addView(_printButton);

   _printButton->log();



    SidebarSceneController::onWillAppear();

/*
    ModelView* modelView = new ModelView(Rect(0,0,270,240));
    modelView->setJobName("Buddahz");
    modelView->setImageFileName("22B92B2B");
    modelView->setColor(ILI9341_CYAN);
    addView(modelView);

    modelView = new ModelView(Rect(270,0,270,240));
    modelView->setJobName("LCD Case");
    modelView->setImageFileName("666.RAW");
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


    */
}


void ProjectsScene::onSidebarButtonTouchUp() {
  SettingsSceneController * scene = new SettingsSceneController();
  Application.pushScene(scene);
}


void ProjectsScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  if (_printButton != NULL) {
    _printButton->setVisible(false);
  }
  SceneController::handleTouchMoved(point, oldPoint);
}


void ProjectsScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);

  //We should have stopped at a defined slot index, use that to position the button
  int index = getPageIndex();
  float x = Display.getLayoutWidth() * index;

  _printButton->setFrame(Rect((x+50),200,170,30));
  _printButton->setVisible(true);
  _printButton->setDelegate(this);
}

void ProjectsScene::buttonPressed(void *button)
{
  /*
  if (button == _printButton) {
    bool jobExists = false;

    if (jobExists) {
      LOG_VALUE("Printing Job-Nr",getPageIndex());
      PrintStatusSceneController * scene = new PrintStatusSceneController();
      Application.pushScene(scene);
    } else {
      LOG_VALUE("Need to download file",getPageIndex());
      DownloadFileController* scene = new DownloadFileController("http://printrapp.s3-us-west-1.amazonaws.com/u/1526cae4477c387ffc07bd6cad001614/p/26c8a30b51efe91ceb507d443600d6d7/f631d66d1df76827a25776fcd9e3ed14.raw", "feet.png");
      Application.pushScene(scene);
    }
  }
  */
  SidebarSceneController::buttonPressed(button);
}
