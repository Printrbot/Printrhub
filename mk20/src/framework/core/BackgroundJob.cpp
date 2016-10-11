//
// Created by Phillip Schuster on 11.10.16.
//

#include "BackgroundJob.h"

BackgroundJob::BackgroundJob() {
  _finished = false;
}

BackgroundJob::~BackgroundJob()
{

}

void BackgroundJob::loop() {

}

void BackgroundJob::onWillEnd() {

}

void BackgroundJob::onWillStart() {

}

void BackgroundJob::exit() {
  _finished = true;
}

bool BackgroundJob::handlesTask(TaskID taskID)
{
  //Let Application do the hard work
  return false;
}

bool BackgroundJob::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool* sendResponse, bool* success)
{
  return true;
}