#ifndef PRINT_STATUS_SCENE
#define PRINT_STATUS_SCENE

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/ProgressBar.h"
#include "framework/views/LabelView.h"
#include "framework/views/LabelButton.h"
#include "scenes/projects/ProjectsScene.h"
#include "scenes/projects/JobsScene.h"
#include "../../Printr.h"

class PrintStatusScene: public SidebarSceneController, public ButtonDelegate, public PrintrListener {

public:
  virtual void loop() override;
  PrintStatusScene(String jobFilePath, Project project, Job job);
  virtual ~PrintStatusScene();

  virtual void onNewNozzleTemperature(float temp);
  virtual void onPrintProgress(float progress);
  virtual void onPrintComplete(bool success);

private:
  virtual UIBitmap * getSidebarBitmap() override;
  virtual UIBitmap * getSidebarIcon() override;
  virtual bool isModal() override;
  virtual void onWillAppear() override;
  virtual void buttonPressed(void *button) override;
  String getName();
  LabelButton* _button;
  ProgressBar* _progressBar;
  SDBitmapLayer* _imageLayer;
  float _step;
  Project _project;
  Job _job;

  String _jobFilePath;
  String _projectIndex;
  int _totalJobLines;

  TextLayer* _nameLayer;
  TextLayer* _printTime;
  TextLayer* _resolution;
  TextLayer* _infill;
  TextLayer* _support;
  TextLayer* _filament;


};

#endif
