/*
 * Updated the MK20 firmware update via Serial Wire Debug protocol. Uses previously
 * stored MK20 firmware binary downloaded to SPIFFS (see DownloadFileToSPIFFS)
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
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

#ifndef MK20FIRMWAREUPDATE_H
#define MK20FIRMWAREUPDATE_H

#include "core/Mode.h"
#include "arm_kinetis_debug.h"

class MK20FirmwareUpdate : public Mode {
 public:
  MK20FirmwareUpdate(String localFilePath);
  virtual ~MK20FirmwareUpdate();

  virtual void loop();
  virtual void onWillStart();
  virtual void onWillEnd();
  virtual bool handlesTask(TaskID taskID);
  virtual String getName();

 private:
  void flashFirmware();
  String _localFilePath;
  ARMKinetisDebug target;
  File firmware_file;
  MK20 *_mk20;
};

#endif //MK20FIRMWAREUPDATE_H
