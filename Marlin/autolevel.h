#ifndef AUTOLEVEL_H
#define AUTOLEVEL_H

#include "Arduino.h"
#include "pbserial.h"
#include "printer.h"

class Autolevel {
public:
  Autolevel();
  static void setupForEndstopMove();
  static void cleanupAfterEndstopMove();
  static void runZProbe();
  static void doBlockingMove(float x, float y, float z);
  static void run();
  static void setBedLevelEquation(float z_at_xLeft_yFront, float z_at_xRight_yFront, float z_at_xLeft_yBack);

};


#endif
