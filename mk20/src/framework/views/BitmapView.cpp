#include "BitmapView.h"

BitmapView::BitmapView(Rect frame):
View(frame.x, frame.y, frame.width, frame.height) {

}

String BitmapView::getDescription() {
	return "BitmapView";
}

void BitmapView::setBitmap(UIBitmap * bitmap) {
  _sdbitmapLayer = new SDBitmapLayer(_frame);
  _sdbitmapLayer->setBitmap("ui.min", bitmap->width, bitmap->height, bitmap->offset);
  _sdbitmapLayer->setContext(getContext());
  addLayer(_sdbitmapLayer);
}
