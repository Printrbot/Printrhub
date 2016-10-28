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
extern int lastJobIndex;
extern int lastProjectIndex;

JobsScene::JobsScene(String projectIndex):
    SidebarSceneController::SidebarSceneController(),
    _projectIndex(projectIndex),
    _jobs(NULL) {
}

JobsScene::JobsScene():
    SidebarSceneController::SidebarSceneController(),
    _jobs(NULL) {
  const char * pdirname = "/projects";
  File pdir = SD.open(pdirname);
  pdir.rewindDirectory();


  for (int cnt = 0; true; ++cnt) {

    File pfile = pdir.openNextFile();
    if (!pfile) break;

    if (pfile.isDirectory()) continue;

    if (lastProjectIndex == cnt) {
      _projectIndex = pdirname;
      _projectIndex += "/";
      _projectIndex += pfile.name();
    }
  }
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


  _printBtnDownload = new BitmapButton(Rect(-100,180,uiBitmaps.btn_print_download.width,uiBitmaps.btn_print_download.height));
  _printBtnStart = new BitmapButton(Rect(-100,180,uiBitmaps.btn_print_start.width,uiBitmaps.btn_print_start.height));

  _printBtnStart->setBitmap(&uiBitmaps.btn_print_start);
  _printBtnDownload->setBitmap(&uiBitmaps.btn_print_download);

  addView(_printBtnStart);
  addView(_printBtnDownload);

  SidebarSceneController::onWillAppear();
}

void JobsScene::onDidAppear() {
  if (lastJobIndex > 0) {
    float x = lastJobIndex * Display.getLayoutWidth();
    addScrollOffset(-x);
  }
  updateButtons();
}

void JobsScene::updateButtons() {
  float x = Display.getLayoutWidth() * lastJobIndex;
  _jobFilePath = "/jobs/" + String(_project.index) + "/" + String(_selectedJob.index);
//_printBtn->setFrame(Rect((x + 80), 190, uiBitmaps.btn_print_start.width, uiBitmaps.btn_print_start.height));
  if (SD.exists(_jobFilePath.c_str())) {
    _printBtnStart->setFrame(Rect((x + 10), 180, uiBitmaps.btn_print_start.width, uiBitmaps.btn_print_start.height));
    _printBtnStart->setVisible(true);
    _printBtnStart->setDelegate(this);
    _printBtnDownload->setVisible(false);
  } else {
    _printBtnDownload->setFrame(Rect((x + 10), 180, uiBitmaps.btn_print_download.width, uiBitmaps.btn_print_download.height));
    _printBtnDownload->setVisible(true);
    _printBtnDownload->setDelegate(this);
    _printBtnStart->setVisible(false);
  }
}

void JobsScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  SceneController::handleTouchMoved(point, oldPoint);
}


void JobsScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
  //We should have stopped at a defined slot index, use that to position the button
  lastJobIndex = getPageIndex();
  // check if local job file exists
  _selectedJob = _jobs[getPageIndex()];

  updateButtons();
}

void JobsScene::onSidebarButtonTouchUp() {
  ProjectsScene * scene = new ProjectsScene();
  Application.pushScene(scene);
}

void JobsScene::buttonPressed(void *button)
{
  if (button == _printBtnStart) {
    LOG_VALUE("Printing Job-Nr",getPageIndex());
    PrintStatusScene * scene = new PrintStatusScene(_jobFilePath, _project, _selectedJob);
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
    // return to job after done...
    DownloadFileController* scene = new DownloadFileController(String(_selectedJob.url), _jobFilePath, _jobFilePath, _project, _selectedJob);
    Application.pushScene(scene);
  }

  SidebarSceneController::buttonPressed(button);
}
