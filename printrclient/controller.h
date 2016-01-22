#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Arduino.h"
#include <ArduinoJson.h>
//#include "net.h"
//#include "printer.h"



class Net;
class Printer;

class Controller {
public:

  static void runJson(JsonObject * o);
  static void fromPrinter(char * c);


};

#endif
