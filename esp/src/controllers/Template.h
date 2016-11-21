/*
 * Template class that we used for new mode based classes (create new file, copy and
 * paste this content and you have a good starting point for your own class)
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban
 * https://github.com/Printrbot/Printrhub
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

#ifndef RUNGCODE_H
#define RUNGCODE_H

#include "core/Mode.h"

class Template : public Mode {
 private:
  typedef enum State {
	StateSuccess = 1,
	StateError = 2,
	StateWaiting = 3
  };

  TaskID AvailableTasks[0]{};

 public:
  Template();

  virtual void loop();
  virtual void onWillStart();
  virtual void onWillEnd();

  virtual String getName();

  bool runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool *success);

 private:
  State _state;
};

#endif
