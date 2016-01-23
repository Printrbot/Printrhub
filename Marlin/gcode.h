#ifndef GCODE_H
#define GCODE_H

#include "Arduino.h"

class Gcode {
public:
  Gcode();
  void setBlock(char * block);
  bool hasCode(char code);
  float getCodeValue();
  long getLongCodeValue();
  void parseCoordinates();
  int current_line;

private:
  char * block;
  char * cpoint;


};



#endif
