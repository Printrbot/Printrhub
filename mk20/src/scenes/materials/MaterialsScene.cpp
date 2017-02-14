/*
 * Shows user interface that shows different materials the user can select to
 * set up print speed, temperature and various other settings without messing
 * around with these settings
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation with Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "MaterialsScene.h"
#include "framework/views/LabelButton.h"
#include "SD.h"
#include "UIBitmaps.h"
#include "scenes/settings/SettingsScene.h"
#include "MaterialView.h"
#include "scenes/settings/DataStore.h"
#include "framework/layers/TransparentTextLayer.h"
#include "font_LiberationSans.h"

extern DataStore dataStore;
extern UIBitmaps uiBitmaps;

MaterialsScene::MaterialsScene() :
	SidebarSceneController::SidebarSceneController(),
	_materials(NULL) {

}

MaterialsScene::~MaterialsScene() {
  if (_materials != NULL) {
	free(_materials);
	_materials = NULL;
  }
}

uint16_t MaterialsScene::getBackgroundColor() {
  return Application.getTheme()->getColor(BackgroundColor);
}

String MaterialsScene::getName() {
  return "MaterialsScene";
}

UIBitmap *MaterialsScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_materials;
}

UIBitmap *MaterialsScene::getSidebarIcon() {
  return &uiBitmaps.btn_exit;
}

void MaterialsScene::onWillAppear() {
  setScrollSnap(Display.getLayoutWidth(), SnapMode::Flick);

  Display.disableAutoLayout();

  String path = "/matlib";
  File _file = SD.open(path.c_str(), FILE_READ);
  _file.seek(33);
  uint8_t _materialsCount;
  _file.read(&_materialsCount, 1);

  MaterialView *materialView;

  _materials = (Material *) malloc(sizeof(Material) * _materialsCount);

  for (uint8_t cnt = 0; cnt < _materialsCount; cnt++) {
	_file.seek(34 + (81 * cnt));
	_file.read(&_materials[cnt], 81);
	materialView = new MaterialView(_materials[cnt], cnt);
	addView(materialView);
  }
  _selectedMaterial = _materials[0];
  _savedMaterial = dataStore.getLoadedMaterial();
  _file.close();


  _selectButton = new BitmapButton(Rect(18, 178, uiBitmaps.btn_select.width, uiBitmaps.btn_select.height));
  _selectButton->setBitmap(&uiBitmaps.btn_select);
  _selectButton->setVisible(false);
  _selectButton->setDelegate(this);
  addView(_selectButton);

  _selectedMsg = new BitmapView(Rect(18, 178, uiBitmaps.selected_msg.width, uiBitmaps.selected_msg.height));
  _selectedMsg->setBitmap(&uiBitmaps.selected_msg);
  _selectedMsg->setVisible(false);
  addView(_selectedMsg);

  updateButtons();

  SidebarSceneController::onWillAppear();
}

void MaterialsScene::onDidAppear() {

}

void MaterialsScene::updateButtons() {
  float x = Display.getLayoutWidth() * getPageIndex();

  if (strcmp(_savedMaterial->name, _selectedMaterial.name) == 0) {
    _selectButton->setVisible(false);
    _selectedMsg->setVisible(true);
    _selectedMsg->setFrame(Rect(x + 18, 178, uiBitmaps.selected_msg.width, uiBitmaps.selected_msg.height));
  } else {
    _selectedMsg->setVisible(false);
    _selectButton->setFrame(Rect(x + 18, 178, uiBitmaps.btn_select.width, uiBitmaps.btn_select.height));
    _selectButton->setVisible(true);
    _selectButton->setDelegate(this);
  }

}

void MaterialsScene::handleTouchMoved(TS_Point point, TS_Point oldPoint) {
  SceneController::handleTouchMoved(point, oldPoint);
}

void MaterialsScene::animationFinished(Animation *animation) {
  SceneController::animationFinished(animation);
  _selectedMaterial = _materials[getPageIndex()];
  updateButtons();
}

void MaterialsScene::onSidebarButtonTouchUp() {
  SettingsScene *scene = new SettingsScene();
  Application.pushScene(scene);
}

void MaterialsScene::buttonPressed(void *button) {

  if (button == _selectButton) {
	dataStore.setLoadedMaterial(_selectedMaterial);
	dataStore.save();
    _savedMaterial = dataStore.getLoadedMaterial();
	//SettingsScene *scene = new SettingsScene();
	//Application.pushScene(scene, true);
    updateButtons();

	return;
  }

  SidebarSceneController::buttonPressed(button);
}

