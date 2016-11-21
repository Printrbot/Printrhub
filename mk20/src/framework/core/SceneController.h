/*
 * Scene controller base class. Handles user interface and does actual work in
 * its loop function.
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/printrbot-simple-2016-display-system-explained
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation by Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

class SceneController : public AnimatableObject {
#pragma mark Constructor
 public:
  virtual void animationUpdated(Animation *animation, float currentValue, float deltaValue, float timeLeft) override;

  virtual void animationFinished(Animation *animation) override;

  SceneController();
  virtual ~SceneController();

#pragma mark Application Flow
  virtual void loop();
  virtual void onWillAppear();
  virtual void onDidAppear();
  virtual void setupDisplay();
  virtual void onWillDisappear();
  virtual bool handlesTask(TaskID taskID);
  virtual bool runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success);
  virtual bool isModal();

#pragma mark Misc
  virtual String getName() = 0;

#pragma mark View Management
  virtual StackArray<View *> *getViews() { return &_views; };
  virtual void addView(View *view) { _views.push(view); };
  virtual View *getView(uint16_t n) { return _views.at(n); };

#pragma mark Touch Handling
  virtual void handleTouchDown(TS_Point &point);
  virtual void handleTouchUp(TS_Point &point);
  virtual void handleTouchMoved(TS_Point point, TS_Point oldPoint);

#pragma mark Getter/Setter
  virtual uint16_t getBackgroundColor();

#pragma mark Scrolling
 protected:
  void addScrollOffset(float scrollOffset);
  void setScrollOffset(float scrollOffset);
 private:
  virtual void setDecelerationRate(const float decelerationRate) { _decelerationRate = decelerationRate; };
 public:
  virtual void setScrollSnap(const float scrollSnap, const SnapMode snapMode) {
	_scrollSnap = scrollSnap;
	_snapMode = snapMode;
  };
  virtual float getScrollSnapTileSize() const { return _scrollSnap; };
  virtual SnapMode getSnapMode() const { return _snapMode; };
  virtual uint16_t getPageIndex();

#pragma mark Member Variables
 private:
  StackArray<View *> _views;
  View *_currentTouchedView;
  float _scrollOffset;
  float _scrollVelocity;
  float _scrollSnap;
  SnapMode _snapMode;
  float _decelerationRate;
  float _currentDecelerationRate;
  Animation *_scrollAnimation;
  unsigned long _lastScrollTime;
};

#endif //__SCENECONTROLLER_H_
