#include "Template.h"
//#include "../core/Commstack.h"
#include "Idle.h"
#include "Application.h"
#include "../event_logger.h"

Template::Template():
_state(StateSuccess) {

};


void Template::loop()
{
  if (_state == StateSuccess) {
    // ...
  }
}

void Template::onWillStart()
{

}

void Template::onWillEnd()
{

}

String Template::getName() {
  return "Template";
}


bool Template::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool *sendResponse, bool* success)
{
  return false;
}
