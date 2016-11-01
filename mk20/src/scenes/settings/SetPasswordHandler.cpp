//
// Created by Phillip Schuster on 01.11.16.
//

#include "SetPasswordHandler.h"
#include "SettingsScene.h"

void SetPasswordHandler::onCancelled() {
  //Do nothing, just get back to settings
  SettingsScene* scene = new SettingsScene();
  Application.pushScene(scene);
}

void SetPasswordHandler::onFinished(String textEntered) {
  if (textEntered.length() == 0) {
    FLOW_NOTICE("Clearing password");
    Application.getESPStack()->requestTask(TaskID::SetPassword,0,NULL);
  } else {
    FLOW_NOTICE("Setting password, length: %d",textEntered.length());
    Application.getESPStack()->requestTask(TaskID::SetPassword,textEntered.length(),(uint8_t*)textEntered.c_str());
  }

  SettingsScene* scene = new SettingsScene();
  Application.pushScene(scene);
}