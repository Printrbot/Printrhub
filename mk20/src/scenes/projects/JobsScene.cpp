#include "JobsScene.h"

#include "framework/views/LabelButton.h"
#include "ImageView.h"
#include "ProjectsScene.h"
#include "SD.h"
#include "UIBitmaps.h"
//#include "font_AwesomeF080.h"
//#include "font_AwesomeF000.h"

extern UIBitmaps uiBitmaps;

JobsScene::JobsScene(String projectIndex):
  SidebarSceneController::SidebarSceneController(),
  _projectIndex(projectIndex) {
}

JobsScene::~JobsScene() {
}


String JobsScene::getName() {
  return "JobsScene";
}

UIBitmap * JobsScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_jobs;
}


UIBitmap * JobsScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

void JobsScene::onWillAppear() {
  setScrollSnap(Display.getLayoutWidth(), SnapMode::Flick);

  //As the model views are distributed as opaque, seamless tiles we don't need auto layout as we don't have spaces where
  //background shines through
  Display.disableAutoLayout();

  // open the file
  File _file = SD.open(_projectIndex.c_str(), FILE_READ);
  // read how many jobs it contains
  uint8_t t;
  _file.seek(72);
  _file.read(&t, 1);
  // loop through jobs and build images

  //Display.fillRect(50,0,270,240,ILI9341_WHITE);

  ImageView* imageView;

  /*
  imageView = new ImageView(Rect(0,0,270,240), 129841);
  imageView->setImageTitle("TESTING");
  imageView->setIndexFileName(_projectIndex.c_str());
  addView(imageView);
  */


  for (uint8_t cnt=0; cnt < t; cnt++) {
    imageView = new ImageView(Rect(270*cnt,0,270,240), 129673 + (129768 * cnt) + 168);

    char _jname[32];
    _file.seek(129673 + (129768 * cnt) + 8 );
    _file.read(_jname, 32);

    imageView->setImageTitle(_jname);
    imageView->setIndexFileName(_projectIndex.c_str());
    addView(imageView);
  }

  _file.close();

  _printBtn = new BitmapButton(Rect(72,175,120,50));
  _printBtn->setBitmap(&uiBitmaps.btn_print);
  _printBtn->setVisible(true);
  _printBtn->setDelegate(this);
  addView(_printBtn);


  SidebarSceneController::onWillAppear();
  //Display.drawBitmap(0,0,50,190, imageOfJobsText_50_190,0,0,50,190);
}


void JobsScene::onSidebarButtonTouchUp() {
  ProjectsScene * scene = new ProjectsScene();
  Application.pushScene(scene);
}


void JobsScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  if (_printBtn != NULL) {
    _printBtn->setVisible(false);
  }
  SceneController::handleTouchMoved(point, oldPoint);
}


void JobsScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
  //We should have stopped at a defined slot index, use that to position the button
  int index = getPageIndex();
  float x = Display.getLayoutWidth() * index;

  //_printBtn->setIcon(imageOfOpenButton_65_65, Application.getTheme()->getColor(HighlightBackgroundColor), 65, 65);
  _printBtn->setFrame(Rect((x+72),175,120,50));
  _printBtn->setVisible(true);
  _printBtn->setDelegate(this);

}

void JobsScene::buttonPressed(void *button)
{

  if (button == _printBtn) {

    ImageView * v = (ImageView *) getView(getPageIndex());
    String fn = v->getIndexFileName();



    /*
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
    */
  }

  SidebarSceneController::buttonPressed(button);
}
