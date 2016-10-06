//
// Created by Phillip Schuster on 09.07.16.
//

#ifndef _TEENSYCMAKE_OBJECT_H_
#define _TEENSYCMAKE_OBJECT_H_

#include "Arduino.h"

class Object
{
public:
	virtual String getDescription() = 0;
};


#endif //_TEENSYCMAKE_OBJECT_H_
