#include "JobsScene.h"

#include "framework/views/LabelButton.h"
#include "ImageView.h"
#include "ProjectsScene.h"
#include "SD.h"
#include "UIBitmaps.h"
#include "../DownloadFileController.h"
#include "scenes/print/PrintStatusScene.h"
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

  _file.seek(32);
  _file.read(&_project, sizeof(Project));

  ImageView* imageView;

  //Create an array holding Job instances
  _jobs = (Job*)malloc(sizeof(Job)* _project.jobs);

  for (uint8_t cnt=0; cnt < _project.jobs; cnt++) {
    imageView = new ImageView(Rect(270*cnt,0,270,240), 129675 + (129899 * cnt) + 299);

    //Read data from file in current Job instance
    _file.seek(129675 + (129899 * cnt));
    _file.read(&_jobs[cnt],299);

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

    //We know how long the filename will be, as /jobs/ (6) + project index (8) + "/" (1) + job index (8) have constant width, add +1 for null terminator!
    //This is a bad hack that shouldn't be necessary as soon as job index is fixed

    String filePath = "/jobs/" + String(_project.index) + "/" + String(job.index);


    //memcpy(&filePath[6],job.index,sizeof(char)*8); //Append job index

    bool jobExists = SD.exists(filePath.c_str());

    if (jobExists) {
      LOG_VALUE("Printing Job-Nr",getPageIndex());
      //PrintStatusScene * scene = new PrintStatusScene(filePath, _project, job, getPageIndex());
      //Application.pushScene(scene);
      //DownloadFileController* scene = new DownloadFileController(String(job.url),filePath);
      //Application.pushScene(scene);
    } else {
      LOG_VALUE("Need to download file",getPageIndex());
      // create directory if needed
      String _jdir = String("/jobs/"+ String(_project.index));
      if (!SD.exists(_jdir.c_str())) {
        SD.mkdir(_jdir.c_str());
      }

      DownloadFileController* scene = new DownloadFileController(String(job.url),filePath);
      Application.pushScene(scene);
    }
  }
  SidebarSceneController::buttonPressed(button);
}
