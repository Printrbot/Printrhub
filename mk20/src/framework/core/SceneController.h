//
// Created by Phillip Schuster on 09.07.16.
//
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
	virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);
	virtual bool isModal();

#pragma mark Misc
	virtual String getName() = 0;

#pragma mark View Management
	virtual StackArray<View*>* getViews() { return &_views; };
	virtual void addView(View* view) { _views.push(view); };
	virtual View* getView(uint16_t n) { return _views.at(n); };

#pragma mark Touch Handling
	virtual void handleTouchDown(TS_Point& point);
	virtual void handleTouchUp(TS_Point& point);
	virtual void handleTouchMoved(TS_Point point, TS_Point oldPoint);

#pragma mark Getter/Setter
	virtual uint16_t getBackgroundColor();

#pragma mark Scrolling
protected:
	void addScrollOffset(float scrollOffset);
private:
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
	unsigned long _lastScrollTime;
};


#endif //__SCENECONTROLLER_H_
