#ifndef BITMAP_VIEW_H
#define BITMAP_VIEW_H


#include "View.h"
#include "../layers/RectangleLayer.h"
#include "../layers/BitmapLayer.h"
#include "../layers/SDBitmapLayer.h"

class BitmapView: public View
{

public:
	BitmapView(Rect frame);
	virtual String getDescription() override;
  virtual void setBitmap(UIBitmap * bitmap);

  virtual void display() override;

private:
  BitmapLayer * _bitmapLayer;
	SDBitmapLayer* _sdbitmapLayer;
	UIBitmap* _bitmap;
};



#endif
