#include "NavBar.h"
#include "framework/core/Application.h"

NavBar::NavBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height):
  View(x,y,width,height)
{
  _name = "NavBar";
}

NavBar::NavBar(Rect frame):
  View(frame)
{
  _name = "NavBar";
}

void NavBar::display()
{
    RectangleLayer* bottomLayer = new RectangleLayer(Rect(_frame.x,_frame.y,_frame.width,_frame.height));
    bottomLayer->setBackgroundColor(Application.getTheme()->getColor(SidebarBackgroundColor));
    addLayer(bottomLayer);
    View::display();
}
