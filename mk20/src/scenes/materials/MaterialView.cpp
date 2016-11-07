#include "MaterialView.h"

#include "framework/core/Application.h"
#include "framework/layers/SDBitmapLayer.h"
#include "framework/layers/TransparentTextLayer.h"
#include "font_LiberationSans.h"

extern UIBitmaps uiBitmaps;

MaterialView::MaterialView(Material material, uint8_t offset):
    View(270*offset,0,270,240),
    _offset(offset),
    _material(material)
{
}


void MaterialView::display()
{
  SDBitmapLayer* imageLayer = new SDBitmapLayer(_frame);
  imageLayer->setBitmap("ui.min", uiBitmaps.materials_scene.width, uiBitmaps.materials_scene.height, uiBitmaps.materials_scene.offset);
  addLayer(imageLayer);


  _titleLayer = new TransparentTextLayer(Rect(_frame.x+5, 100, Display.getLayoutWidth()-5,14));
  _titleLayer->setTextAlign(TEXTALIGN_CENTERED);
  _titleLayer->setFont(&LiberationSans_14);
  _titleLayer->setText(_material.name);
  _titleLayer->setForegroundColor(ILI9341_BLACK);
  addLayer(_titleLayer);

  _typeLayer = new TransparentTextLayer(Rect(_frame.x+5,125,Display.getLayoutWidth()-5,14));
  _typeLayer->setTextAlign(TEXTALIGN_CENTERED);
  _typeLayer->setFont(&LiberationSans_12);
  String _l = "Type: ";
  _l = _l + _material.type;
  _typeLayer->setText(_l.c_str());
  _typeLayer->setForegroundColor(ILI9341_BLACK);
  addLayer(_typeLayer);

  /*
  _brandLayer = new TransparentTextLayer(Rect(_frame.x+5,_frame.y+40,Display.getLayoutWidth()-15,230));
  _brandLayer->setTextAlign(TEXTALIGN_CENTERED);
  _brandLayer->setFont(&LiberationSans_12);
  _l = "Brand: ";
  _l = _l + _material.brand;
  _brandLayer->setText(_l.c_str());
  _brandLayer->setForegroundColor(ILI9341_BLACK);
  addLayer(_brandLayer);
*/
  _temperatureLayer = new TransparentTextLayer(Rect(_frame.x+5,150, Display.getLayoutWidth()-5,14));
  _temperatureLayer->setTextAlign(TEXTALIGN_CENTERED);
  _temperatureLayer->setFont(&LiberationSans_12);
  _l = "Print Temp: ";
  _l = _l + (int) _material.temperature;
  _temperatureLayer->setText(_l.c_str());
  _temperatureLayer->setForegroundColor(ILI9341_BLACK);
  addLayer(_temperatureLayer);


  View::display();
}
