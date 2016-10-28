#include "BitmapView.h"

BitmapView::BitmapView(Rect frame):
View(frame.x, frame.y, frame.width, frame.height),
_bitmap(NULL),
_bitmapLayer(NULL),
_sdbitmapLayer(NULL)
{

}

String BitmapView::getDescription() {
	return "BitmapView";
}

void BitmapView::setBitmap(UIBitmap * bitmap) {
  _bitmap = bitmap;

  if (_sdbitmapLayer != NULL) {
    _sdbitmapLayer->setBitmap("ui.min", bitmap->width, bitmap->height, bitmap->offset);
  }
}

void BitmapView::display() {
  if (!_bitmap) return;

  if (!_sdbitmapLayer) {
    _sdbitmapLayer = new SDBitmapLayer(_frame);
    _sdbitmapLayer->setBitmap("ui.min", _bitmap->width, _bitmap->height, _bitmap->offset);
    _sdbitmapLayer->setContext(getContext());
    addLayer(_sdbitmapLayer);
  }

  View::display();
}