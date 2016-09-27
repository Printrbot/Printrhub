//
// Created by Phillip Schuster on 20.01.16.
//

#ifndef TEENSYCMAKE_BITMAPBUTTON_H
#define TEENSYCMAKE_BITMAPBUTTON_H

#include "View.h"
#include "../layers/BitmapLayer.h"
#include "../layers/SDBitmapLayer.h"
#include "Button.h"
#include "../core/UIBitmap.h"

class BitmapButton;

class BitmapButton: public View, public Button
{
#pragma mark Constructor
public:
    BitmapButton(Rect frame);
    virtual ~BitmapButton();

#pragma mark Getter/Setter
    virtual void setBitmap(const uint8_t* bitmap, uint16_t width, uint16_t height);
    virtual void setBitmap(const uint16_t* bitmap, uint16_t width, uint16_t height);
    virtual void setBitmap(UIBitmap * bitmap);
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

#pragma mark View
    virtual void setFrame(Rect frame, bool updateLayout=true) override;
private:
    void createBitmapFrame(Rect* frame, uint16_t width, uint16_t height);
#pragma mark Touch Handling
private:
    virtual bool touchUp(TS_Point &point) override;
    virtual bool touchDown(TS_Point &point) override;
    virtual void touchCancelled() override;

#pragma mark Member Functions
    void updateButton(ButtonState buttonState);

#pragma mark Member Variables
protected:
    Rect _originalFrame;
    Animation* _touchAnimation;
    uint16_t _color;
    uint16_t _alternateBackgroundColor;
    uint16_t _alternateTextColor;
    RectangleLayer* _baseLayer;
    BitmapLayer* _bitmapLayer;
    SDBitmapLayer* _sdbitmapLayer;
};


#endif //TEENSYCMAKE_BITMAPBUTTON_H
