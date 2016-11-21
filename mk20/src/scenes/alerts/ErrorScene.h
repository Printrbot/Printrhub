/*
 * Shows an error dialog to give the user some feedback about errors
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
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


#ifndef MK20_ERRORSCENE_H
#define MK20_ERRORSCENE_H

#include "../SidebarSceneController.h"
#include "framework/views/BitmapButton.h"
#include "framework/views/LabelButton.h"

class ErrorScene : public SidebarSceneController {
 public:

  ErrorScene(const String &errorMessage, bool showButton = true);
  virtual ~ErrorScene();

  virtual void onSidebarButtonTouchUp() override;

  virtual uint16_t getBackgroundColor() override;
  virtual UIBitmap *getSidebarBitmap() override;
  virtual UIBitmap *getSidebarIcon() override;

 private:
  virtual void onWillAppear() override;
  String getName() override;
  virtual void buttonPressed(void *button) override;

 protected:
  LabelButton *_okBtn;
  bool _showButton;
  String _errorMessage;
};

#endif //MK20_ERRORSCENE_H
