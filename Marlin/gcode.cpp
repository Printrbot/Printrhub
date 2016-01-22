#include "gcode.h"
#include "printer.h"

Gcode::Gcode() {
}

void Gcode::setBlock(char * block) {
  this->block = block;
}

bool Gcode::hasCode(char code)
{
  this->cpoint = strchr(this->block, code);
  return (this->cpoint != NULL);
}

float Gcode::getCodeValue()
{
  return (strtod(&this->block[this->cpoint - this->block + 1], NULL));
}

long Gcode::getLongCodeValue()
{
  return (strtol(&this->block[this->cpoint - this->block + 1], NULL, 10));
}

void Gcode::parseCoordinates()
{
  for(int8_t i=0; i < 4; i++) {
    if(this->hasCode(Printer::axis_codes[i]))
    {
      Printer::destination[i] = (float) this->getCodeValue() + (Printer::axis_relative[i] || Printer::relative_mode)* Printer::current_position[i];
    }
  }
  if(this->hasCode('F')) {
    float f = this->getCodeValue();
    if(f > 0.0)
      Printer::feedrate = f;
  }
}
