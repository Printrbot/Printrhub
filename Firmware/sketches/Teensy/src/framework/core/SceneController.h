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

#include "../views/View.h"
#include "Application.h"
#include "../../framework/animation/Animation.h"
#include "CommStack.h"

typedef enum SnapMode {
	Disabled = 0,
	OneByOne = 1,
	Flick = 2
};

class SceneController: public AnimatableObject
{
#pragma mark Constructor
public:
	virtual void animationUpdated(Animation *animation, float currentValue, float deltaValue, float timeLeft) override;

	virtual void animationFinished(Animation *animation) override;

	SceneController();
	virtual ~SceneController();

#pragma mark Application Flow
	virtual void loop();
	virtual void onWillAppear();
	virtual void setupDisplay();
	virtual void onWillDisappear();
	virtual bool handlesTask(TaskID taskID);
	virtual bool runTask(CommHeader& header, const uint8_t* data, uint8_t* responseData, uint16_t* responseDataSize);

#pragma mark Misc
	virtual String getName() = 0;

#pragma mark View Management
	virtual StackArray<View*>* getViews() { return &_views; };
	virtual void addView(View* view) { _views.push(view); };

#pragma mark Touch Handling
	virtual void handleTouchDown(TS_Point& point);
	virtual void handleTouchUp(TS_Point& point);
	virtual void handleTouchMoved(TS_Point point, TS_Point oldPoint);

#pragma mark Getter/Setter
	virtual uint16_t getBackgroundColor();

#pragma mark Scrolling
private:
	void addScrollOffset(float scrollOffset);
	virtual void setDecelerationRate(const float decelerationRate) { _decelerationRate = decelerationRate; };
public:
	virtual void setScrollSnap(const float scrollSnap, const SnapMode snapMode) { _scrollSnap = scrollSnap; _snapMode = snapMode; };
	virtual float getScrollSnapTileSize() const { return _scrollSnap; };
	virtual SnapMode getSnapMode() const { return _snapMode; };
	virtual uint16_t getPageIndex();

#pragma mark Member Variables
private:
	StackArray<View*> _views;
	View* _currentTouchedView;
	float _scrollOffset;
	float _scrollVelocity;
	float _scrollSnap;
	SnapMode _snapMode;
	float _decelerationRate;
	float _currentDecelerationRate;
	Animation* _scrollAnimation;
};


#endif //__SCENECONTROLLER_H_
