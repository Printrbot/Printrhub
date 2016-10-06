//
// Created by Phillip Schuster on 25.06.16.
//

#include "../event_logger.h"
#include "Idle.h"

Idle::Idle()
{

}

Idle::~Idle()
{

}

void Idle::loop()
{
}

void Idle::onWillStart()
{
  EventLogger::log("Starting IDLE mode...");
}

void Idle::onWillEnd()
{

}

String Idle::getName()
{
    return "Idle";
}
