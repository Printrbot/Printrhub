//
// Created by Phillip Schuster on 20.01.16.
//

#ifndef TEENSYCMAKE_BITMAPBUTTON_H
#define TEENSYCMAKE_BITMAPBUTTON_H

#include "View.h"
#include "BitmapLayer.h"
#include "Button.h"

class BitmapButton;

class BitmapButton: public View, public Button
{
public:
    BitmapButton(Rect frame);
    virtual ~BitmapButton();

    virtual void setBitmap(const uint8_t* bitmap, uint16_t width, uint16_t height);

    virtual void setFrame(Rect frame) override;

    uint16_t getColor() const { return _color; }
    void setColor(uint16_t color) { _color = color; }

    virtual void setAlternateBackgroundColor(uint16_t color) { _alternateBackgroundColor = color; };
    virtual uint16_t getAlternateBackgroundColor() { return _alternateBackgroundColor; };;;;

    void setAlternateTextColor(uint16_t color) { _alternateTextColor = color; };
    uint16_t getAlternateTextColor() { return _alternateTextColor; };

    RectangleLayer *getBaseLayer() const { return _baseLayer; }
    void setBaseLayer(RectangleLayer *baseLayer) { _baseLayer = baseLayer; }

    BitmapLayer *getBitmapLayer() const { return _bitmapLayer; }
    void setBitmapLayer(BitmapLayer *bitmapLayer) { _bitmapLayer = bitmapLayer; }

    void updateButton(ButtonState buttonState);

private:
    virtual bool touchUp(TS_Point &point) override;
    virtual bool touchDown(TS_Point &point) override;
    virtual void touchCancelled() override;

protected:
    Rect _originalFrame;
    Animation* _touchAnimation;
    uint16_t _color;

    uint16_t _alternateBackgroundColor;
    uint16_t _alternateTextColor;

    RectangleLayer* _baseLayer;
    BitmapLayer* _bitmapLayer;
};


#endif //TEENSYCMAKE_BITMAPBUTTON_H
