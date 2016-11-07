#ifndef MATERIAL_VIEW_H
#define MATERIAL_VIEW_H

#include "../../framework/views/View.h"
#include "../../framework/layers/TextLayer.h"
#include "../../framework/layers/RectangleLayer.h"
#include "../../framework/layers/BitmapLayer.h"
#include "../../framework/layers/SDBitmapLayer.h"
#include "../../framework/views/LabelButton.h"


typedef struct Material {
  char name[32];
  char type[12];
  char brand[32];
  int16_t temperature;
  float speed;
  bool retraction;
} Material;


class MaterialView: public View
{
public:
  MaterialView(Material material, uint8_t offset = 0);

  uint16_t _width;
  virtual void display() override;

private:
  Material _material;
  uint32_t _offset;
  TextLayer* _titleLayer;
  TextLayer* _typeLayer;
  TextLayer* _brandLayer;
  TextLayer* _temperatureLayer;
  TextLayer* _speedLayer;
};


#endif
