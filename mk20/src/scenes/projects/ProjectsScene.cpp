#include "ProjectsScene.h"
#include "framework/views/BitmapButton.h"
#include "ImageView.h"
#include "SD.h"
#include "../settings/SettingsScene.h"
//#include "../print/PrintStatusSceneController.h"
//#include "../DownloadFileController.h"
#include "../SidebarSceneController.h"
#include "ConfirmDeleteProject.h"
#include "JobsScene.h"
#include "NoProjects.h"

extern UIBitmaps uiBitmaps;
extern int lastProjectIndex;
extern int lastJobIndex;
extern int totalProjects;

ProjectsScene::ProjectsScene(): SidebarSceneController::SidebarSceneController(),
_totalProjects(0) {
}

ProjectsScene::~ProjectsScene() {
}


String ProjectsScene::getName() {
  return "ProjectsScene";
}

UIBitmap * ProjectsScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_project;
}

UIBitmap * ProjectsScene::getSidebarIcon() {
  return &uiBitmaps.btn_settings;
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

  ImageView* imageView;

  for (int cnt = 0; true; ++cnt) {

    File pfile = pdir.openNextFile();
    if (!pfile) break;

    if (pfile.isDirectory()) continue;

    imageView = new ImageView(Rect(270 * cnt,0,270,240), 73);

    char _pname[32];
    pfile.seek(42);
    pfile.read(_pname, 32);

    imageView->setImageTitle(_pname);

    String img = pdirname;
    img += "/";
    img += pfile.name();

    imageView->setIndexFileName(img.c_str());
    addView(imageView);
    pfile.close();

    _totalProjects++;
  }

  totalProjects = _totalProjects;
  if (_totalProjects == 0) {
    NoProjects * scene = new NoProjects();
    Application.pushScene(scene);
    return;
  }

  // return to the last viewed project index
  if (lastProjectIndex > 0) {
    float x = Display.getLayoutWidth() * lastProjectIndex;
    _openBtn = new BitmapButton(Rect(x+10,180,uiBitmaps.btn_open.width,uiBitmaps.btn_open.height));
    _deleteBtn = new BitmapButton(Rect(x+220,190,50,50));
    addScrollOffset(-x);
  } else{
    _openBtn = new BitmapButton(Rect(10,180,uiBitmaps.btn_open.width,uiBitmaps.btn_open.height));
    _deleteBtn = new BitmapButton(Rect(220,190,50,50));
  }

  _openBtn->setBitmap(&uiBitmaps.btn_open);
  _openBtn->setVisible(true);
  _openBtn->setDelegate(this);
  addView(_openBtn);

  _deleteBtn->setBitmap(&uiBitmaps.btn_trash);
  _deleteBtn->setVisible(true);
  _deleteBtn->setDelegate(this);
  addView(_deleteBtn);

  SidebarSceneController::onWillAppear();
}


void ProjectsScene::onSidebarButtonTouchUp() {
  SettingsScene * scene = new SettingsScene();
  Application.pushScene(scene);
}

void ProjectsScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  //_openBtn->setVisible(false);
  //_deleteBtn->setVisible(false);
  SceneController::handleTouchMoved(point, oldPoint);
}

void ProjectsScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);

  //We should have stopped at a defined slot index, use that to position the button
  lastProjectIndex = getPageIndex();
  lastJobIndex = 0;

  float x = Display.getLayoutWidth() * lastProjectIndex;

  _openBtn->setFrame(Rect(x+10,180,uiBitmaps.btn_open.width,uiBitmaps.btn_open.height));
  _openBtn->setVisible(true);
  _openBtn->setDelegate(this);

  _deleteBtn->setFrame(Rect(x+220,190,50,50));
  _deleteBtn->setVisible(true);
  _deleteBtn->setDelegate(this);
}

void ProjectsScene::buttonPressed(void *button)
{

  if (button == _openBtn) {

    ImageView * v = (ImageView *) getView(getPageIndex());
    String fn = v->getIndexFileName();

    JobsScene * js = new JobsScene(fn);
    Application.pushScene(js);

/*
    getView(getPageIndex());

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

  } else if (button == _deleteBtn) {
    ImageView * v = (ImageView *) getView(getPageIndex());
    String fn = v->getIndexFileName();

    ConfirmDeleteProject * scene = new ConfirmDeleteProject(fn);
    Application.pushScene(scene);
  }

  SidebarSceneController::buttonPressed(button);
}
