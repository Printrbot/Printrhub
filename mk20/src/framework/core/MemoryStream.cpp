/*
 * MemoryStream implements the Stream protocol as a pure memory solution. Use it
 * for writing to a memory area with Streams convenience functions like print
 * and println
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Phillip Schuster
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation by Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com/printrhub
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

#include "MemoryStream.h"

MemoryStream::MemoryStream() {
  _posRead = 0;
  _posWrite = 0;
  _chunkSize = 64;
  _autoFlush = true;
  _buffer = (uint8_t *) malloc(sizeof(uint8_t) * _chunkSize);
  _len = _chunkSize;
  clearBuffer();
}

MemoryStream::MemoryStream(size_t initialLength) {
  _buffer = (uint8_t *) malloc(sizeof(uint8_t) * initialLength);
  _len = initialLength;
  _posRead = 0;
  _posWrite = 0;
  _chunkSize = 64;
  _autoFlush = true;
  clearBuffer();
}

MemoryStream::MemoryStream(uint8_t *buffer, size_t len) {
  _buffer = (uint8_t *) malloc(sizeof(uint8_t) * len);
  memcpy(_buffer, buffer, len);
  _posRead = 0;
  _posWrite = len;
  _len = len;
  _chunkSize = 64;
  _autoFlush = true;
  clearBuffer();
}

MemoryStream::~MemoryStream() {
  if (_buffer != NULL) {
	free(_buffer);
	_buffer = NULL;
  }
}

size_t MemoryStream::write(uint8_t byte) {
  if (_posWrite >= _len) {
	if (!resizeBuffer(_len + _chunkSize)) {
	  return 0;
	}
  }

  _buffer[_posWrite] = byte;
  _posWrite++;
  return 1;
}

int MemoryStream::peek() {
  if (!available()) return -1;

  return _buffer[_posRead];
}

int MemoryStream::read() {
  int byte = peek();
  if (byte >= 0) {
	_posRead++;
  } else {
	//If we cannot read because nothing is left, auto purge it by flushing it back to default
	if (_autoFlush) {
	  if (_posWrite == _posRead && _posWrite > 0) {
		flush();
	  }
	}
  }

  return byte;
}

int MemoryStream::available() {
  int pos = _posWrite - _posRead;
  if (pos < 0) pos = 0;
  return pos;
}

bool MemoryStream::resizeBuffer(size_t newBufferSize) {
  if (_len == newBufferSize) return true;

  uint8_t *newBuffer = (uint8_t *) realloc(_buffer, newBufferSize);
  if (newBuffer != NULL) {
	_buffer = newBuffer;
	_len = newBufferSize;
	return true;
  }

  return false;
}

void MemoryStream::clearBuffer() {
  if (_buffer != NULL) {
	memset(_buffer, 0, _len);
  }
}

const char *MemoryStream::c_str() {
  return (const char *) _buffer;
}

void MemoryStream::flush() {
  if (_len > _chunkSize) {
	resizeBuffer(_chunkSize);
  }

  _posRead = 0;
  _posWrite = 0;

  clearBuffer();
}