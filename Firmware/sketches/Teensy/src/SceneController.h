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

#ifndef __SCENECONTROLLER_H_
#define __SCENECONTROLLER_H_

#include "View.h"
#include "Application.h"

class SceneController
{
public:
	SceneController();
	virtual ~SceneController();

	virtual void loop();

	virtual String getName() = 0;

	//Events
	virtual void onWillAppear();
	virtual void onWillDisappear();

	bool statusBarVisible();

	virtual StackArray<View*>* getViews() { return &_views; };
	virtual void addView(View* view) { _views.push(view); };

	virtual void handleTouchDown(TS_Point& point);
	virtual void handleTouchUp(TS_Point& point);
	virtual void handleTouchMoved(TS_Point point, TS_Point oldPoint);

	virtual uint16_t getBackgroundColor();

	virtual void dismiss() { _dismissed = true; };
	virtual bool isDismissed() { return _dismissed; };

private:
	void addScrollOffset(float scrollOffset);

	StackArray<View*> _views;
	View* _currentTouchedView;
	float _scrollOffset;
	float _scrollVelocity;
	bool _dismissed;
};


#endif //__SCENECONTROLLER_H_
