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

  _selectedJob = _jobs[0];
  _jobFilePath = "/jobs/" + String(_project.index) + "/" + String(_selectedJob.index);

  _printBtn = new BitmapButton(Rect(80,190,uiBitmaps.btn_print_start.width,uiBitmaps.btn_print_start.height));
  if (SD.exists(_jobFilePath.c_str())) {
    _printBtn->setBitmap(&uiBitmaps.btn_print_start);
  } else {
    _printBtn->setBitmap(&uiBitmaps.btn_print_download);
  }
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

  // check if local job file exists
  _selectedJob = _jobs[getPageIndex()];
  _jobFilePath = "/jobs/" + String(_project.index) + "/" + String(_selectedJob.index);

  _printBtn->setFrame(Rect((x + 80), 190, uiBitmaps.btn_print_start.width, uiBitmaps.btn_print_start.height));
  if (SD.exists(_jobFilePath.c_str())) {
    _printBtn->setBitmap(&uiBitmaps.btn_print_start);
  } else {
    _printBtn->setBitmap(&uiBitmaps.btn_print_download);
  }
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
    if (SD.exists(_jobFilePath.c_str())) {
      LOG_VALUE("Printing Job-Nr",getPageIndex());
      PrintStatusScene * scene = new PrintStatusScene(_jobFilePath, _project, _selectedJob, getPageIndex());
      Application.pushScene(scene);
      //DownloadFileController* scene = new DownloadFileController(String(job.url),filePath);
      //Application.pushScene(scene);
    } else {
      LOG_VALUE("Need to download file",getPageIndex());
      // create directory if needed
      String _jdir = String("/jobs/"+ String(_project.index));
      if (!SD.exists(_jdir.c_str())) {
        SD.mkdir(_jdir.c_str());
      }

      DownloadFileController* scene = new DownloadFileController(String(_selectedJob.url),_jobFilePath);
      Application.pushScene(scene);
    }
  }
  SidebarSceneController::buttonPressed(button);
}
