/*
 * Little Helper -- your best friend in your lab
 * http://www.appfruits.com/littlehelper
 *
 * This software is part of the firmware running Little Helper. You may use, copy, 
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software 
 * but you must include this copyright notice and this permission in all copies or 
 * substantial portions of the Software.
 *
 * Appfruits invests time and resources to make this open source. Please support us
 * and the open-source hardware initiative.
 * 
 * Copyright 2014-2015 Phillip Schuster (@appfruits)
 * http://www.appfruits.com
 * 
 * MIT-License
 */

#ifndef _VIEW_H_
#define _VIEW_H_

#include <Adafruit_FT6206.h>
#include "Arduino.h"
#include "Animation.h"
#include "Object.h"
#include "UIElement.h"
#include "StackArray.h"
#include "Layer.h"

#define MINX 0
#define MAXX 128
#define MINY 25
#define MAXY 160
#define MAXH (MAXY-MINY)
#define MAXW (MAXX-MINX)

class View: public UIElement, public AnimatableObject, Object
{
public:
	~View();
	View(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
	View(Rect frame);

	Rect getScreenFrame();

	//Display
	virtual void draw();
	virtual void update();
	virtual void layout();
	void setBackgroundColor(uint16_t backgroundColor);
	void setOpaque(bool opaque);
	virtual void display();

	virtual void setNeedsDisplay();
	void didDraw();

	//Layer handling
	virtual void addLayer(Layer* layer);

	//Touch handling
	virtual bool touchDown(TS_Point& point);
	virtual bool touchMoved(TS_Point& point, TS_Point& lastPoint);
	virtual bool touchUp(TS_Point& point);
	virtual void touchCancelled();

	//Getters and Settings
	bool isUserInteractionEnabled() const
	{
		return _userInteractionEnabled;
	}

	void setUserInteractionEnabled(bool userInteractionEnabled)
	{
		_userInteractionEnabled = userInteractionEnabled;
	}

	//Visibility
	bool isVisible();
	void setVisible(bool visible=true);

	virtual View* hitTest(TS_Point& point);

protected:
	virtual void animationUpdated(Animation *animation, float currentValue, float timeLeft) override;

protected:
	Rect _frame;
	bool _visible;
	bool _opaque;
	uint16_t _backgroundColor;
	bool _needsDisplay;
	StackArray<Layer*> _layers;
	bool _userInteractionEnabled;

public:
	virtual String getDescription() override;

	virtual void setFrame(Rect frame) override;
};

#endif
