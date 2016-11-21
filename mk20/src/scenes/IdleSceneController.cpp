/*
 * Shows an empty screen
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation with Phillip Schuster (@appfruits) from appfruits.com
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

#include "IdleSceneController.h"

IdleSceneController::IdleSceneController() :
	_view(IdleView(Rect(0, 0, 320, 240))),
	_size(10) {
  _velocity = 1;
}

String IdleSceneController::getName() {
  return "idleSceneController";
}

void IdleSceneController::display() {

}

void IdleSceneController::onWillAppear() {
  SceneController::onWillAppear();

  RectangleLayer *layer = _view.getLayer();
  layer->setFrame(Rect(50, 50, 20, 20));

  _view.getLayer2()->setFrame(Rect(120, 120, 30, 30));

  _view.display();
}

void IdleSceneController::loop() {
  _view.getLayer()->setFrame(Rect(50 + _size, 50 + _size, 20, 20));
  _view.getLayer2()->setFrame(Rect(120 - _size, 120 + _size * 2, 30 + _size, 30 + _size));
  _size += _velocity;
  if (_size > 50 || _size < -30) {
	_velocity = -_velocity;
  }
}