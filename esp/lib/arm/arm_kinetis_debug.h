/*
 * Simple ARM debug interface for Arduino, using the SWD (Serial Wire Debug) port.
 * Extensions for Freescale Kinetis chips.
 *
 * Copyright (c) 2013 Micah Elizabeth Scott
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

#pragma once
#include "arm_debug.h"
#include "FS.h"

class ARMKinetisDebug : public ARMDebug
{
public:
    ARMKinetisDebug(unsigned clockPin, unsigned dataPin, LogLevel logLevel = LOG_NORMAL);

    // First-time initialization, resetting into system halt state.
    bool startup();

    bool detect();              // Detect supported Kinetis hardware
    bool reset();               // System reset

    // Even more hardware-specific
    bool initK20();             // Initialize K20 peripherals into default state

    // High-level GPIO interface.
    bool pinMode(unsigned p, int mode);
    bool digitalWrite(unsigned p, int value);
    int digitalRead(unsigned p);
    bool digitalWritePort(unsigned port, unsigned value);

    // Bit-band operations on IO space
    bool memStoreBit(uint32_t addr, unsigned bit, uint32_t data);
    bool memLoadBit(uint32_t addr, unsigned bit, uint32_t &data);

    // USB controller pull-up resistor
    bool usbSetPullup(bool enable);

    // Flash mass-erase operation. Works even on protected devices.
    bool flashMassErase();

    // Initialize the FlexRAM buffer for flash sector programming
    bool flashSectorBufferInit();

    // Write a chunk of data to the flash sector buffer
    bool flashSectorBufferWrite(uint32_t bufferOffset, const uint32_t *data, unsigned count);

    // Erases a sector
    bool flashSectorErase(uint32_t address);

    // Write one flash sector from the buffer
    bool flashSectorProgram(uint32_t address);

    bool eraseEverything();

    bool setProtectionBits(bool protect);

    bool dumpSector(uint32_t address);

    /*
     * High-level flash programming manager. Handles the entire programming process,
     * including protection resets and verification.
     */
    class FlashProgrammer {
        public:
            FlashProgrammer(ARMKinetisDebug &target, const uint32_t *image, unsigned numSectors);
            bool begin();
            bool isComplete();
            bool next();
            bool installFirmware(const uint8_t* firmware, uint32_t firmware_length);
            bool installFirmware(File* file);

        private:
            ARMKinetisDebug &target;
            const uint32_t *image;
            unsigned numSectors;
            unsigned nextSector;
            bool isVerifying;
    };

    class Flasher {
    public:
        Flasher(ARMKinetisDebug &target);
        bool installFirmware(File* file);

    private:
        ARMKinetisDebug &target;
        File* file;
        bool begin();
        bool end();
        bool next();
        bool next2();
        uint32_t address;
        uint8_t sector;
        int sectorSizeInBytes;
        int byteAlignment;
    };

    static const uint32_t kFlashSectorSize = 1024;

    // Port constants. (Corresponds to PCR address base)
    enum Port {
        PTA = 0x0000,
        PTB = 0x1000,
        PTC = 0x2000,
        PTD = 0x3000,
        PTE = 0x4000,
    };

    // Pin constants. (Corresponds to PCR address map)
    enum Pin {
        PTA0    = 0x0000 + 4 * 0,
        PTA1    = 0x0000 + 4 * 1,
        PTA2    = 0x0000 + 4 * 2,
        PTA3    = 0x0000 + 4 * 3,
        PTA4    = 0x0000 + 4 * 4,
        PTA5    = 0x0000 + 4 * 5,
        PTA12   = 0x0000 + 4 * 12,
        PTA13   = 0x0000 + 4 * 13,
        PTA18   = 0x0000 + 4 * 18,
        PTA19   = 0x0000 + 4 * 19,
        PTB0    = 0x1000 + 4 * 0,
        PTB1    = 0x1000 + 4 * 1,
        PTB2    = 0x1000 + 4 * 2,
        PTB3    = 0x1000 + 4 * 3,
        PTB16   = 0x1000 + 4 * 16,
        PTB17   = 0x1000 + 4 * 17,
        PTB18   = 0x1000 + 4 * 18,
        PTB19   = 0x1000 + 4 * 19,
        PTC0    = 0x2000 + 4 * 0,
        PTC1    = 0x2000 + 4 * 1,
        PTC2    = 0x2000 + 4 * 2,
        PTC3    = 0x2000 + 4 * 3,
        PTC4    = 0x2000 + 4 * 4,
        PTC5    = 0x2000 + 4 * 5,
        PTC6    = 0x2000 + 4 * 6,
        PTC7    = 0x2000 + 4 * 7,
        PTC8    = 0x2000 + 4 * 8,
        PTC9    = 0x2000 + 4 * 9,
        PTC10   = 0x2000 + 4 * 10,
        PTC11   = 0x2000 + 4 * 11,
        PTD0    = 0x3000 + 4 * 0,
        PTD1    = 0x3000 + 4 * 1,
        PTD2    = 0x3000 + 4 * 2,
        PTD3    = 0x3000 + 4 * 3,
        PTD4    = 0x3000 + 4 * 4,
        PTD5    = 0x3000 + 4 * 5,
        PTD6    = 0x3000 + 4 * 6,
        PTD7    = 0x3000 + 4 * 7,
        PTE0    = 0x4000 + 4 * 0,
        PTE1    = 0x4000 + 4 * 1,
    };

    // Assemble a pin number programmatically
    Pin pin(Port port, unsigned index) {
        return (Pin)(port + index * 4);
    }

protected:
    bool testMemoryAccess();



    // Low-level flash interface
    bool ftfl_busyWait();
    bool ftfl_launchCommand();
    bool ftfl_setFlexRAMFunction(uint8_t controlCode);
    bool ftfl_programSection(uint32_t address, uint32_t numLWords);
    bool ftfl_handleCommandStatus(const char *cmdSpecificError = 0);
    bool ftfl_programLongword(uint32_t address, const uint8_t* bytes);
    bool ftfl_programPartitionFunction(uint8_t eepromSize, uint8_t partitionCode);
};
