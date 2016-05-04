#ifndef TEENSYCMAKE_NAVBAR_H
#define TEENSYCMAKE_NAVBAR_H

#include "View.h"

class NavBar: public View
{
public:
  NavBar(Rect frame);
  NavBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

  virtual void display() override;
};

#endif
