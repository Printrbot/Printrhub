//
// Created by Phillip Schuster on 17.10.16.
//

#include "MemoryStream.h"

MemoryStream::MemoryStream() {
  _posRead = 0;
  _posWrite = 0;
  _chunkSize = 64;
  _buffer = (uint8_t*)malloc(sizeof(uint8_t)*_chunkSize);
  _len = _chunkSize;
}

MemoryStream::MemoryStream(size_t initialLength) {
  _buffer = (uint8_t*)malloc(sizeof(uint8_t)*initialLength);
  _len = initialLength;
  _posRead = 0;
  _posWrite = 0;
  _chunkSize = 64;
}

MemoryStream::MemoryStream(uint8_t *buffer, size_t len) {
  _buffer = (uint8_t*)malloc(sizeof(uint8_t)*len);
  memcpy(_buffer,buffer,len);
  _posRead = 0;
  _posWrite = len;
  _len = len;
  _chunkSize = 64;
}

MemoryStream::~MemoryStream() {
  if (_buffer != NULL) {
    free(_buffer);
    _buffer = NULL;
  }
}

size_t MemoryStream::write(uint8_t byte) {
  if (_posWrite >= _len) {
    if (!resizeBuffer(_len+_chunkSize)) {
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

  uint8_t* newBuffer = (uint8_t*)realloc(_buffer,newBufferSize);
  if (newBuffer != NULL) {
    _buffer = newBuffer;
    _len = newBufferSize;
    return true;
  }

  return false;
}

void MemoryStream::clearBuffer() {
  if (_buffer != NULL) {
    memset(_buffer,0,_len);
  }
}

void MemoryStream::flush() {
  if (_len > _chunkSize) {
    if (resizeBuffer(_chunkSize)) {
      clearBuffer();
    }
  }

  _posRead = 0;
  _posWrite = 0;
}