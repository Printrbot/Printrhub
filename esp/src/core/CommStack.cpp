/*
 * Bidirectional, asynchronous, fast , reliable request/response based protocol
 * based on UART (Serial) connections
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/printrbot-simple-2016-commstack-explained
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

#include "CommStack.h"
#include "Application.h"

CommStack::CommStack(Stream *port, CommStackDelegate *delegate) :
	_port(port),
	_delegate(delegate),
	_expectedPacketType(Header),
	_receiveBufferIndex(0),
	_packetMarker(COMM_STACK_PACKET_MARKER),
	_ready(false) {
  pinMode(COMMSTACK_DATAFLOW_PIN, INPUT);
}

CommStack::~CommStack() {
}

bool CommStack::prepareResponse(CommHeader *commHeader, bool success) {
  //Flip Request to Response, if this is a Response switch to next task. If all tasks
  //have been processed just return true and do nothing
  if (commHeader->commType == Request) {
	if (success) {
	  commHeader->commType = ResponseSuccess;
	} else {
	  commHeader->commType = ResponseFailed;
	}

	return true;
  }

  return false;
}

void CommStack::log(const char *msg, ...) {
  char buffer[500];
  va_list args;
  va_start(args, msg);
  sprintf(buffer, msg, args);
  va_end(args);

  int len = strlen(buffer);
  requestTask(TaskID::DebugLog, (size_t) (len + 1), (uint8_t *) &buffer[0]);
}

/*
 * Taken from PacketSerial COBS encoding (https://github.com/bakercp/PacketSerial/blob/master/src/Encoding/COBS.h)
 */
size_t CommStack::encode(const uint8_t *source, size_t size, uint8_t *destination) {
  size_t read_index = 0;
  size_t write_index = 1;
  size_t code_index = 0;
  uint8_t code = 1;

  while (read_index < size) {
	if (source[read_index] == 0) {
	  destination[code_index] = code;
	  code = 1;
	  code_index = write_index++;
	  read_index++;
	} else {
	  destination[write_index++] = source[read_index++];
	  code++;

	  if (code == 0xFF) {
		destination[code_index] = code;
		code = 1;
		code_index = write_index++;
	  }
	}
  }

  destination[code_index] = code;

  return write_index;
}

/*
 * Taken from PacketSerial COBS encoding (https://github.com/bakercp/PacketSerial/blob/master/src/Encoding/COBS.h)
 */
size_t CommStack::decode(const uint8_t *source, size_t size, uint8_t *destination) {
  size_t read_index = 0;
  size_t write_index = 0;
  uint8_t code;
  uint8_t i;

  while (read_index < size) {
	code = source[read_index];

	if (read_index + code > size && code != 1) {
	  return 0;
	}

	read_index++;

	for (i = 1; i < code; i++) {
	  destination[write_index++] = source[read_index++];
	}

	if (code != 0xFF && read_index != size) {
	  destination[write_index++] = '\0';
	}
  }

  return write_index;
}

/*
 * Taken from PacketSerial COBS encoding (https://github.com/bakercp/PacketSerial/blob/master/src/Encoding/COBS.h)
 */
size_t CommStack::getEncodedBufferSize(size_t sourceSize) {
  return sourceSize + sourceSize / 254 + 1;
}

void CommStack::send(const uint8_t *buffer, size_t size, bool sendMarker) {
  if (_port == 0 || buffer == 0 || size == 0) return;
  if (!isReady()) return;

  //Data flow pin is LOW
  unsigned long startWaitTime = millis();
  if (digitalRead(COMMSTACK_DATAFLOW_PIN) == LOW) {
	LOG("Waiting for Ready To Send Signal");
	while (digitalRead(COMMSTACK_DATAFLOW_PIN) == LOW) {
	  ESP.wdtFeed();
	  delayMicroseconds(1);

	  //Don't wait forever that MK20 comes back
	  if (millis() - startWaitTime > 20000) {
		_ready = false;
		Application.setMK20Timeout();
		break;
	  }
	}
	LOG("Signal received, sending...");
  }

  uint8_t _encodeBuffer[getEncodedBufferSize(size)];
  size_t numEncoded = encode(buffer, size, _encodeBuffer);

  LOG("Sending encoded data");
  _port->write(_encodeBuffer, numEncoded);

  if (sendMarker) {
	_port->write(_packetMarker);
  }

  _port->flush();
}

void CommStack::runTask(const uint8_t *buffer, size_t size) {
  LOG("Running task and preparing response buffer");
  //Clear response data buffer
  memset(_sendBuffer, 0, COMM_STACK_BUFFER_SIZE);

  //Trigger the application to run the task and send responded data
  uint16_t responseDataSize = 0;
  uint8_t *responseBuffer = &_sendBuffer[sizeof(CommHeader)];
  bool sendResponse = true;
  bool success = true;
  _delegate->runTask(_currentHeader, buffer, size, responseBuffer, &responseDataSize, &sendResponse, &success);

  LOG_VALUE("Running task complete, Response data size", responseDataSize);
  //Prepare header for the response
  if (sendResponse && prepareResponse(&_currentHeader, success)) {
	if (_currentHeader.commType == Request) {
	  LOG_VALUE("Sending Request", _currentHeader.getCurrentTask());
	} else {
	  LOG_VALUE("Sending Response", _currentHeader.getCurrentTask());
	}

	//Set size of responded data
	_currentHeader.contentLength = responseDataSize;
	uint16_t checkSum = 0;
	if (responseDataSize > 0) {
	  checkSum = getCheckSum(responseBuffer, responseDataSize);
	}
	_currentHeader.setDataCheckSum(checkSum);

	//Copy the header
	memcpy(_sendBuffer, &_currentHeader, sizeof(CommHeader));

	//Send data
	send(_sendBuffer, responseDataSize + sizeof(CommHeader), true);
  } else {
	LOG("Task sequence complete");
  }
}

void CommStack::packetReceived(const uint8_t *buffer, size_t size) {
  //Copy header into struct and test checksum
  memcpy(&_currentHeader, buffer, sizeof(CommHeader));

  //Now check if calculated checksum is equal that was sent
  if (_currentHeader.isOK()) {
	if (_currentHeader.contentLength > 0) {
	  //We have data attached
	  uint8_t *data = (uint8_t *) &buffer[sizeof(CommHeader)];
	  size_t dataSize = size - sizeof(CommHeader);

	  uint16_t checkSum = getCheckSum(data, _currentHeader.contentLength);
	  if (checkSum == _currentHeader.dataCheckSum) {
		runTask(data, dataSize);
	  } else {
		EventLogger::log("Data checksums do not match, received malformed packet");
	  }
	} else {
	  //We have no data attached
	  runTask(NULL, 0);
	}
  } else {
	EventLogger::log("Header checksums not ok, received malformed packet");
  }
}

void CommStack::process() {
  //Make sure data flow pin on other side is HIGH, meaning that CommStack on the other side has been initialized
  if (!_ready) {
	if (digitalRead(COMMSTACK_DATAFLOW_PIN) == HIGH) {
	  _ready = true;
	} else {
	  return;
	}
  }

  while (_port->available() > 0) {
	uint8_t data = _port->read();

	if (data == COMM_STACK_PACKET_MARKER) {
	  LOG_VALUE("Packet received, decoding number of bytes", _receiveBufferIndex);
	  uint8_t _decodeBuffer[_receiveBufferIndex];
	  size_t numDecoded = decode(_receiveBuffer, _receiveBufferIndex, _decodeBuffer);
	  _receiveBufferIndex = 0;

	  LOG_VALUE("Handling decoded packet with size", numDecoded);
	  //Packet received
	  packetReceived(_decodeBuffer, numDecoded);
	} else {
	  if ((_receiveBufferIndex + 1) < COMM_STACK_BUFFER_SIZE) {
		_receiveBuffer[_receiveBufferIndex++] = data;
	  } else {
		// Error, buffer overflow if we write.
	  }
	}
  }
}

uint16_t CommStack::getCheckSum(const uint8_t *data, size_t size) {
  uint16_t checkSum = 0;
  for (int i = 0; i < size; i++) {
	checkSum += data[i];
  }
  return checkSum;
}

bool CommStack::sendMessage(CommHeader &header, size_t contentLength, const uint8_t *data) {
  if (_port == 0) return false;

  //Calculate the data checksum and set the checksums of header
  uint16_t checkSum = getCheckSum(data, contentLength);
  header.setDataCheckSum(checkSum);

  //Calculate size of packet (header + data)
  size_t size = contentLength + sizeof(CommHeader);

  EventLogger::log("Sending message with taskID: %d, content length: %d, total size: %d", header.getCurrentTask(), contentLength, size);

  //Prepare packet in memory
  memcpy(_sendBuffer, &header, sizeof(CommHeader));
  if (contentLength > 0) {
	memcpy(&_sendBuffer[sizeof(CommHeader)], data, contentLength);
  }

  //Send data
  send(_sendBuffer, size, true);

  return true;
}

bool CommStack::requestTask(TaskID task, size_t contentLength, const uint8_t *data) {
  LOG_VALUE("Request Task without data with ID", task);
  CommHeader header(task, contentLength);

  return sendMessage(header, contentLength, data);
}

bool CommStack::requestTask(TaskID task, const JsonObject &object) {
  int length = object.measureLength() + 1;
  char buffer[length];
  object.printTo(buffer, length);

  EventLogger::log("Sending JSON for Task %d with length: %d", task, length);
  EventLogger::log(buffer);

  return requestTask(task, length, (uint8_t *) buffer);
}

bool CommStack::requestTask(TaskID task) {
  LOG_VALUE("Request Task without data with ID", task);
  CommHeader header(task, 0);

  return sendMessage(header);
}

bool CommStack::responseTask(TaskID task, size_t contentLength, const uint8_t *data, bool success) {
  LOG_VALUE("Response Task with data with ID", task);
  CommHeader header(task, contentLength);

  //Set as response
  if (success) {
	header.commType = ResponseSuccess;
  } else {
	header.commType = ResponseFailed;
  }

  return sendMessage(header, contentLength, data);
}

bool CommStack::responseTask(TaskID task, bool success) {
  LOG_VALUE("Response Task without data with ID", task);
  CommHeader header(task, 0);

  //Set as response
  if (success) {
	header.commType = ResponseSuccess;
  } else {
	header.commType = ResponseFailed;
  }

  return sendMessage(header);
}
