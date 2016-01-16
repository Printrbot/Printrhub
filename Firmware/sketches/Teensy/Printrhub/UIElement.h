//
// Created by Phillip Schuster on 16.01.16.
//

#ifndef TEENSYCMAKE_UIELEMENT_H
#define TEENSYCMAKE_UIELEMENT_H


#include "Arduino.h"

struct Rect
{
public:
    Rect() {};
    Rect(int x, int y, int w, int h) { this->x=x; this->y=y; this->width=w; this->height=h;};

    int midY() { return (y + height)/2; };
    int midX() { return (x + width)/2; };
    int right() { return (x + width); };
    int bottom() { return (y + height); };
    int top() { return (y); };

    int x;
    int y;
    int width;
    int height;
};

class UIElement
{
public:
    void setFrame(Rect frame);
    void setFrame(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    Rect& getFrame();

    virtual void display() = 0;

protected:
    Rect _frame;
};


#endif //TEENSYCMAKE_UIELEMENT_H
