#include "ImageView.h"
#include "framework/core/Application.h"
#include "framework/layers/SDBitmapLayer.h"
#include "framework/layers/TransparentTextLayer.h"
#include "font_LiberationSans.h"

ImageView::ImageView(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t offset):
  View(x,y,width,height),
  _offset(offset)
{
  _name = "ImageView";
}

ImageView::ImageView(Rect frame, uint32_t offset):
  View(frame),
  _offset(offset)
{

}

void ImageView::display()
{
  SDBitmapLayer* imageLayer = new SDBitmapLayer(_frame);
  imageLayer->setBitmap(_indexFileName.c_str(), 270, 240, _offset);
  addLayer(imageLayer);

  _imageLayer = imageLayer;

  _imageTitleLayer = new TransparentTextLayer(Rect(_frame.x+15,_frame.y+10,Display.getLayoutWidth()-30,25));
  _imageTitleLayer->setTextAlign(TEXTALIGN_LEFT);
  _imageTitleLayer->setFont(&LiberationSans_14);
  _imageTitleLayer->setText(_imageTitle);
  _imageTitleLayer->setForegroundColor(ILI9341_BLACK);
  //_imageTitleLayer->setBackgroundColor(getBackgroundColor());
  addLayer(_imageTitleLayer);
  View::display();
}
