/*
 * Base class of all layers taking part in the display system.
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

#ifndef TEENSYCMAKE_LAYER_H
#define TEENSYCMAKE_LAYER_H

#include "../core/UIElement.h"
#include "../core/StackArray.h"

class Layer : public UIElement {
#pragma mark Constructor
 public:
  Layer();
  Layer(Rect frame);
  virtual ~Layer();

#pragma mark Getter/Setter
  void setBackgroundColor(const uint16_t &color);
  uint16_t getBackgroundColor() const;

  void setStrokeColor(const uint16_t &strokeColor);
  uint16_t getStrokeColor() const;

  void setStrokeWidth(const uint8_t strokeWidth);
  uint8_t getStrokeWidth() const;

#pragma mark Layout Management
  void splitWithRect(Rect &rect);
  void splitVertically(int x, Layer **left, Layer **right);
  void splitHorizontally(int y, Layer **top, Layer **bottom);
  void invalidateRect(Rect &invalidationRect);
  uint16_t getOriginX();

#pragma mark Layer Hierarchy
  void addSublayer(Layer *layer);
  StackArray<Layer *> *getSublayers();
  bool isLeaf();
  void removeAllSublayers();
  virtual Layer *subLayerWithRect(Rect frame);

#pragma mark Draw and Display
  virtual void setNeedsDisplay();
  virtual void draw(Rect &invalidationRect);
  void display(Layer *backgroundLayer = NULL);
  Rect getRenderFrame();
  bool isVisible();
  void setVisible(const bool visible);
  virtual Rect prepareRenderFrame(const Rect proposedRenderFrame);

#pragma mark Misc
  void log();

#pragma mark UIElements
  virtual void setFrame(Rect frame, bool updateLayout = true) override;

#pragma mark Member Variables
 protected:
  uint16_t _backgroundColor;
  uint16_t _strokeColor;
  uint8_t _strokeWidth;
  bool _needsDisplay;
  int uniqueId;
  StackArray<Layer *> *_sublayers;
  bool _visible;
};

#endif //TEENSYCMAKE_LAYER_H
