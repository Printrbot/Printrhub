#ifndef MK20_MATERIALSSCENE_H
#define MK20_MATERIALSSCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelView.h"
#include "framework/views/LabelButton.h"
#include "MaterialView.h"

class MaterialsScene: public SidebarSceneController
{

public:
  MaterialsScene();
  virtual ~MaterialsScene();

  virtual void handleTouchMoved(TS_Point point, TS_Point oldPoint) override;
  virtual void animationFinished(Animation *animation) override;
  virtual void onSidebarButtonTouchUp() override;
  virtual UIBitmap * getSidebarBitmap() override;
  virtual UIBitmap * getSidebarIcon() override;


private:
  virtual void onWillAppear() override;
  virtual void onDidAppear() override;
  String getName() override;
  virtual void buttonPressed(void *button) override;
  Material* _materials;
  Material _selectedMaterial;
  void updateButtons();

protected:
  BitmapButton* _selectButton;
};



#endif
