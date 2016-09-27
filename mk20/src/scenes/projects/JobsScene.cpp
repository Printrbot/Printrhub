#include "JobsScene.h"

#include "framework/views/LabelButton.h"
#include "ImageView.h"
#include "ProjectsScene.h"
#include "SD.h"
#include "UIBitmaps.h"
#include "../DownloadFileController.h"
//#include "font_AwesomeF080.h"
//#include "font_AwesomeF000.h"

extern UIBitmaps uiBitmaps;

JobsScene::JobsScene(String projectIndex):
  SidebarSceneController::SidebarSceneController(),
  _projectIndex(projectIndex),
  _jobs(NULL){
}

JobsScene::~JobsScene() {
    if (_jobs != NULL) {
        free (_jobs);
        _jobs = NULL;
    }
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

  //Create an array holding Job instances
  _jobs = (Job*)malloc(sizeof(Job)*t);

  for (uint8_t cnt=0; cnt < t; cnt++) {
    imageView = new ImageView(Rect(270*cnt,0,270,240), 129673 + (129768 * cnt) + 168);

    //Read data from file in current Job instance
    _file.seek(129673 + (129768 * cnt));
    _file.read(&_jobs[cnt],8+32+128);

    imageView->setImageTitle(_jobs[cnt].title);
    imageView->setIndexFileName(_projectIndex.c_str());
    addView(imageView);

  }

  _file.close();

  _printBtn = new BitmapButton(Rect(167,190,uiBitmaps.btn_print.width,uiBitmaps.btn_print.height));
  _printBtn->setBitmap(&uiBitmaps.btn_print);
  _printBtn->setVisible(true);
  _printBtn->setDelegate(this);
  addView(_printBtn);


  SidebarSceneController::onWillAppear();
  //Display.drawBitmap(0,0,50,190, imageOfJobsText_50_190,0,0,50,190);
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
  _printBtn->setFrame(Rect((x+167),190,uiBitmaps.btn_print.width,uiBitmaps.btn_print.height));
  _printBtn->setVisible(true);
  _printBtn->setDelegate(this);

}

void JobsScene::onSidebarButtonTouchUp() {
  ProjectsScene * scene = new ProjectsScene();
  Application.pushScene(scene);
}

void JobsScene::buttonPressed(void *button)
{
  if (button == _printBtn) {

    //Query current Job item
    Job job = _jobs[getPageIndex()];

    //This does not work as Job structs index field is 8 bytes but filled with content, there is no byte left for the null terminator,
    //thus this code adds the index and the title to fn (as job.index delivers index+title as index doesn't have a null terminator)
    //TODO: Change job index structure to 9 bytes for index to give room for null terminator
    //String fn = String("/jobs/") + String(job.index);

    //We know how long the filename will be, as /jobs/ (6) and index (8) have constant width, add +1 for null terminator!
    //This is a bad hack that shouldn't be necessary as soon as job index is fixed
    char filePath[15] = "/jobs/";
    memcpy(&filePath[6],job.index,sizeof(char)*8); //Append job index

    bool jobExists = SD.exists(filePath);
    if (jobExists) {
      LOG_VALUE("Printing Job-Nr",getPageIndex());
      //PrintStatusScene * scene = new PrintStatusScene();
      //Application.pushScene(scene);
      DownloadFileController* scene = new DownloadFileController(String(job.url),String(filePath));
      Application.pushScene(scene);
    } else {
      LOG_VALUE("Need to download file",getPageIndex());
      DownloadFileController* scene = new DownloadFileController(String(job.url),String(filePath));
      Application.pushScene(scene);
    }
  }
  SidebarSceneController::buttonPressed(button);
}
