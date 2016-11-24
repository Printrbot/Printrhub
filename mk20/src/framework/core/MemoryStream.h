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

#ifndef MK20_MEMORYSTREAM_H
#define MK20_MEMORYSTREAM_H

#include <inttypes.h>
#include <Stream.h>

class MemoryStream : public Stream {
 public:
  MemoryStream();
  MemoryStream(uint8_t *buffer, size_t len);
  MemoryStream(size_t initialLength);
  ~MemoryStream();

  void setChunkSize(uint16_t chunkSize) { _chunkSize = chunkSize; };

  const size_t length() const { return _posWrite; };

  const char *c_str();

  virtual size_t write(uint8_t byte);
  virtual int read();
  virtual int available();
  int peek();
  void flush();

 private:
  bool resizeBuffer(size_t newBufferSize);
  void clearBuffer();

 private:
  uint8_t *_buffer;
  size_t _len;
  size_t _posWrite;
  size_t _posRead;
  uint16_t _chunkSize;
  bool _autoFlush;
};

#endif //MK20_MEMORYSTREAM_H
