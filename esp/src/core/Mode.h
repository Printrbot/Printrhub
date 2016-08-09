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

#ifndef __MODE_H_
#define __MODE_H_

#include "Arduino.h"
#include "Application.h"
#include "CommStack.h"

class Mode
{
#pragma mark Constructor
public:

	Mode();
	virtual ~Mode();

#pragma mark Application Flow
	virtual void loop();
	virtual void onWillStart();
	virtual void onWillEnd();

#pragma mark Communication with MK20
	virtual bool handlesTask(TaskID taskID);
	virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success);

#pragma mark Misc
	virtual String getName() = 0;

#pragma mark Member Variables
private:

};


#endif //__MODE_H_
