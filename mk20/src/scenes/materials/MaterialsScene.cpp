
#include "MaterialsScene.h"
#include "framework/views/LabelButton.h"
#include "SD.h"
#include "UIBitmaps.h"
#include "scenes/settings/SettingsScene.h"
#include "MaterialView.h"
#include "scenes/settings/DataStore.h"

extern DataStore dataStore;
extern UIBitmaps uiBitmaps;

MaterialsScene::MaterialsScene():
  SidebarSceneController::SidebarSceneController(),
  _materials(NULL) {

}

MaterialsScene::~MaterialsScene() {
  if (_materials != NULL) {
    free (_materials);
    _materials = NULL;
  }
}


String MaterialsScene::getName() {
  return "MaterialsScene";
}

UIBitmap * MaterialsScene::getSidebarBitmap() {
  return &uiBitmaps.sidebar_materials;
}


UIBitmap * MaterialsScene::getSidebarIcon() {
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

  MaterialView* materialView;

  _materials = (Material*)malloc(sizeof(Material) * _materialsCount);

  for (uint8_t cnt=0; cnt < _materialsCount; cnt++) {
    _file.seek(34 + (81 * cnt));
    _file.read(&_materials[cnt],81);
    materialView = new MaterialView(_materials[cnt], cnt);
    addView(materialView);
  }

  _file.close();
  _selectButton = new BitmapButton(Rect(18,178,uiBitmaps.btn_select.width,uiBitmaps.btn_select.height));
  _selectButton->setBitmap(&uiBitmaps.btn_select);
  _selectButton->setVisible(true);
  _selectButton->setDelegate(this);
  addView(_selectButton);

  SidebarSceneController::onWillAppear();
}

void MaterialsScene::onDidAppear() {

  // TODO: read the selected material from eeprom and jump to it
  updateButtons();
}

void MaterialsScene::updateButtons() {
  float x = Display.getLayoutWidth() * getPageIndex();
  _selectButton->setFrame(Rect(x+18,178,uiBitmaps.btn_select.width,uiBitmaps.btn_select.height));
  _selectButton->setVisible(true);
  _selectButton->setDelegate(this);
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
  SettingsScene * scene = new SettingsScene();
  Application.pushScene(scene);
}

void MaterialsScene::buttonPressed(void *button)
{
  if (button == _selectButton) {
    dataStore.setLoadedMaterial(_selectedMaterial);
    dataStore.save();

    SettingsScene * scene = new SettingsScene();
    Application.pushScene(scene, true);
    return;
  }

  SidebarSceneController::buttonPressed(button);
}

