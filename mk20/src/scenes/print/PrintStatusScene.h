#ifndef PRINT_STATUS_SCENE
#define PRINT_STATUS_SCENE

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/ProgressBar.h"
#include "framework/views/LabelView.h"
#include "framework/views/LabelButton.h"
#include "scenes/projects/ProjectsScene.h"
#include "scenes/projects/JobsScene.h"

class PrintStatusScene: public SidebarSceneController, public ButtonDelegate {

public:
  virtual void loop() override;
  PrintStatusScene(String jobFilePath, Project project, Job job, uint16_t offset);
  virtual ~PrintStatusScene();

private:
  virtual UIBitmap * getSidebarBitmap() override;
  virtual UIBitmap * getSidebarIcon() override;

  String getName();
  virtual void onWillAppear() override;
  virtual void printrCallback(const char ctype[], float * data) override;
  virtual void buttonPressed(void *button) override;
  LabelButton* _button;
  ProgressBar* _progressBar;
  SDBitmapLayer* _imageLayer;
  TextLayer* _nameLayer;
  TextLayer* _pLayer;
  float _step;
  Project _project;
  Job _job;

  String _jobFilePath;
  String _projectIndex;
  int _jobOffset;

};

#endif
