/*
 * Base class for all rectangular areas in the display system. Used by views and layers
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

#ifndef TEENSYCMAKE_UIELEMENT_H
#define TEENSYCMAKE_UIELEMENT_H

#include "Arduino.h"

struct Edge {
 public:
  Edge() {};
  Edge(int x, int y, int x1, int y1) {
	this->x = x;
	this->y = y;
	this->x1 = x1;
	this->y1 = y1;
  };

  bool isVertical() { return ((x == x1) ? true : false); };
  bool isHorizontal() { return ((y == y1) ? true : false); };

  int x;
  int y;
  int x1;
  int y1;
};

struct Rect {
 public:
  Rect() {};
  Rect(int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
  };

  int midY() { return (y + height) / 2; };
  int midX() { return (x + width) / 2; };
  int left() { return (x); };
  int right() { return (x + width); };
  int bottom() { return (y + height); };
  int top() { return (y); };

  void setLeft(int left) {
	int diff = x - left;
	x = left;
	width += diff;
  }
  void setTop(int top) {
	int diff = y - top;
	y = top;
	height += diff;
  }
  void setRight(int r) { width += r - right(); }
  void setBottom(int b) { height += b - bottom(); }

  Edge topEdge() { return Edge(x, y, right(), y); };
  Edge leftEdge() { return Edge(x, y, x, bottom()); };
  Edge rightEdge() { return Edge(right(), y, right(), bottom()); };
  Edge bottomEdge() { return Edge(x, bottom(), right(), bottom()); };

  bool containsX(int sx) { return (sx > x && sx < right()); };
  bool containsY(int sy) { return (sy > y && sy < bottom()); };

  bool intersectsRect(Rect &rect) {
	return (rect.left() < right() && rect.right() > left() && rect.top() < bottom() && rect.bottom() > top());
  };

  bool operator==(const Rect &a) const {
	return (x == a.x && y == a.y && width == a.width && height == a.height);
  }

  String toString() { return String(x) + "," + String(y) + " - " + String(width) + "," + String(height); };

  //Outline is not considered as "in" the rectangle
  bool containsPoint(int x, int y) {
	if (x > left() && x < right()) {
	  if (y > top() && y < bottom()) {
		return true;
	  }
	}

	return false;
  };

  bool containsEdge(Edge &edge) {
	if (containsPoint(edge.x, edge.y) || containsPoint(edge.x1, edge.y1)) {
	  return true;
	}

	return false;
  }

  static Rect Intersect(Rect &a, Rect &b) {
	int x = max(a.x, b.x);
	int num1 = min(a.x + a.width, b.x + b.width);
	int y = max(a.y, b.y);
	int num2 = min(a.y + a.height, b.y + b.height);
	if (num1 >= x && num2 >= y)
	  return Rect(x, y, num1 - x, num2 - y);
	else
	  return Rect(0, 0, 0, 0);
  }

  int x;
  int y;
  int width;
  int height;
};

typedef enum DisplayContext {
  Scrolling = 0,
  Fixed = 1
};

class UIElement {
 public:
  virtual void setFrame(Rect frame, bool updateLayout = true);
  void setFrame(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool updateLayout);
  Rect &getFrame();

  void setName(String name) { _name = name; };
  String getName() { return _name; };

  virtual void setContext(const DisplayContext context) { _context = context; }
  DisplayContext getContext() const { return _context; }

 protected:
  Rect _frame;
  String _name;
  DisplayContext _context;
};

#endif //TEENSYCMAKE_UIELEMENT_H
