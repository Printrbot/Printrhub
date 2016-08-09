//
// Created by Phillip Schuster on 09.07.16.
//

#ifndef _VIEW_H_
#define _VIEW_H_

#include <Adafruit_FT6206.h>
#include "Arduino.h"
#include "../../framework/animation/Animation.h"
#include "../../framework/core/Object.h"
#include "../../framework/core/UIElement.h"
#include "../../framework/core/StackArray.h"
#include "../../framework/layers/Layer.h"

#define MINX 0
#define MAXX 128
#define MINY 25
#define MAXY 160
#define MAXH (MAXY-MINY)
#define MAXW (MAXX-MINX)

class View: public UIElement, public AnimatableObject, Object
{
#pragma mark Constructor
public:
	virtual void setContext(const DisplayContext context) override;

	~View();
	View(int x, int y, int width, int height);
	View(Rect frame);

	void log();

#pragma mark UIElement
	virtual String getDescription() override;
	virtual void setFrame(Rect frame) override;

#pragma mark Display
	Rect getScreenFrame();
	virtual void draw();
	virtual void update();
	virtual void layout();
	virtual void display();
	virtual void setNeedsDisplay();
	void didDraw();

#pragma mark Getter/Setter
	void setBackgroundColor(uint16_t backgroundColor);
	uint16_t getBackgroundColor();
	bool isUserInteractionEnabled() const { return _userInteractionEnabled; }
	void setUserInteractionEnabled(bool userInteractionEnabled) { _userInteractionEnabled = userInteractionEnabled; }
	uint16_t getBorderColor() const { return _borderColor; }
	void setBorderColor(uint16_t _borderColor) { View::_borderColor = _borderColor; }
	uint8_t getBorderWidth() const { return _borderWidth; }
	void setBorderWidth(uint8_t _borderWidth) { View::_borderWidth = _borderWidth; }

	void setOpaque(bool opaque);

#pragma mark Layer Management
	virtual void addLayer(Layer* layer);

#pragma mark Touch Handling
	virtual bool touchDown(TS_Point& point);
	virtual bool touchMoved(TS_Point& point, TS_Point& lastPoint);
	virtual bool touchUp(TS_Point& point);
	virtual void touchCancelled();
	virtual View* hitTest(TS_Point& point);

#pragma mark Visibility
	bool isVisible();
	void setVisible(bool visible=true);

#pragma mark Animatable Object
protected:
	virtual void animationUpdated(Animation *animation, float currentValue, float deltaValue, float timeLeft) override;

#pragma mark Member Variables
protected:
	bool _visible;
	bool _opaque;
	uint16_t _backgroundColor;
	uint16_t _borderColor;
	uint8_t _borderWidth;
	bool _needsDisplay;
	StackArray<Layer*> _layers;
	bool _userInteractionEnabled;
};

#endif
