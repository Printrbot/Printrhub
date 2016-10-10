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

  _printBtnDownload = new BitmapButton(Rect(80,190,uiBitmaps.btn_print_download.width,uiBitmaps.btn_print_download.height));
  _printBtnStart = new BitmapButton(Rect(80,190,uiBitmaps.btn_print_start.width,uiBitmaps.btn_print_start.height));

  _printBtnStart->setBitmap(&uiBitmaps.btn_print_start);
  _printBtnDownload->setBitmap(&uiBitmaps.btn_print_download);

  _printBtnStart->setDelegate(this);
  _printBtnDownload->setDelegate(this);

  if (SD.exists(_jobFilePath.c_str())) {
    _printBtnStart->setVisible(true);
    _printBtnDownload->setVisible(false);
  } else {
    _printBtnStart->setVisible(true);
    _printBtnDownload->setVisible(false);
  }

  addView(_printBtnStart);
  addView(_printBtnDownload);

  SidebarSceneController::onWillAppear();
  //Display.drawBitmap(0,0,50,190, imageOfJobsText_50_190,0,0,50,190);
}

void JobsScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  if (_printBtnStart != NULL) {
    _printBtnStart->setVisible(false);
  }
  if (_printBtnDownload != NULL) {
    _printBtnDownload->setVisible(false);
  }
  SceneController::handleTouchMoved(point, oldPoint);
}


void JobsScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
  //We should have stopped at a defined slot index, use that to position the button
  int index = getPageIndex();
  float x = Display.getLayoutWidth() * index;

  // check if local job file exists
  _selectedJob = _jobs[getPageIndex()];
  _jobFilePath = "/jobs/" + String(_project.index) + "/" + String(_selectedJob.index);

  //_printBtn->setFrame(Rect((x + 80), 190, uiBitmaps.btn_print_start.width, uiBitmaps.btn_print_start.height));
  if (SD.exists(_jobFilePath.c_str())) {
    _printBtnStart->setFrame(Rect((x + 80), 190, uiBitmaps.btn_print_start.width, uiBitmaps.btn_print_start.height));
    _printBtnStart->setVisible();
    _printBtnStart->setDelegate(this);
  } else {
    _printBtnDownload->setFrame(Rect((x + 80), 190, uiBitmaps.btn_print_download.width, uiBitmaps.btn_print_download.height));
    _printBtnDownload->setVisible();
    _printBtnDownload->setDelegate(this);
  }
}

void JobsScene::onSidebarButtonTouchUp() {
  ProjectsScene * scene = new ProjectsScene();
  Application.pushScene(scene);
}

void JobsScene::buttonPressed(void *button)
{
  if (button == _printBtnStart) {
    LOG_VALUE("Printing Job-Nr",getPageIndex());
    PrintStatusScene * scene = new PrintStatusScene(_jobFilePath, _project, _selectedJob, getPageIndex());
    Application.pushScene(scene);
    //DownloadFileController* scene = new DownloadFileController(String(job.url),filePath);
    //Application.pushScene(scene);
  } else if (button == _printBtnDownload) {
    LOG_VALUE("Need to download file",getPageIndex());
    // create directory if needed
    String _jdir = String("/jobs/"+ String(_project.index));
    if (!SD.exists(_jdir.c_str())) {
      SD.mkdir(_jdir.c_str());
    }

    DownloadFileController* scene = new DownloadFileController(String(_selectedJob.url),_jobFilePath);
    Application.pushScene(scene);
  }

  SidebarSceneController::buttonPressed(button);
}
