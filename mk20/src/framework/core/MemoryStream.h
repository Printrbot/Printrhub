//
// Created by Phillip Schuster on 17.10.16.
//

#ifndef MK20_MEMORYSTREAM_H
#define MK20_MEMORYSTREAM_H

#include <inttypes.h>
#include <Stream.h>

class MemoryStream: public Stream {
public:
    MemoryStream();
    MemoryStream(uint8_t* buffer, size_t len);
    MemoryStream(size_t initialLength);
    ~MemoryStream();

    void setChunkSize(uint16_t chunkSize) { _chunkSize = chunkSize; };

    const size_t length() const { return _posWrite; };

    const char* c_str();

    virtual size_t write(uint8_t byte);
    virtual int read();
    virtual int available();
    int peek();
    void flush();

private:
    bool resizeBuffer(size_t newBufferSize);
    void clearBuffer();

private:
    uint8_t* _buffer;
    size_t _len;
    size_t _posWrite;
    size_t _posRead;
    uint16_t _chunkSize;
    bool _autoFlush;
};


#endif //MK20_MEMORYSTREAM_H
