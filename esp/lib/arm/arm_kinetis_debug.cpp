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

/*
 * Added longword programming and Flasher class that adds support for Teensy 3.1/3.2
 *
 * Phillip Schuster (@appfruits)
 * http://www.appfruits.com
 *
 * Developed for Printrbot Simple 2016
 * http://www.printrbot.com
 */

#include <Arduino.h>
#include "arm_kinetis_debug.h"
#include "arm_kinetis_reg.h"



ARMKinetisDebug::ARMKinetisDebug(unsigned clockPin, unsigned dataPin, LogLevel logLevel)
    : ARMDebug(clockPin, dataPin, logLevel)
{}

bool ARMKinetisDebug::startup()
{
    return detect() && reset() && debugHalt();
}

bool ARMKinetisDebug::detect()
{
    // Make sure we're on a compatible chip. The MDM-AP peripheral is Freescale-specific.
    uint32_t idr;
    if (!apRead(REG_MDM_IDR, idr))
        return false;

    /*
     * This needs verification, but I think the low half is a version number.
     * The K20 CPU from Fadecandy has an IDR of 0x001c0000,
     * but the Cortex-M0 based KE04Z has an IDR of 0x001c0020. Okay then!
     */

    if ((idr & 0xffff0000) != 0x001C0000) {
        log(LOG_ERROR, "ARMKinetisDebug: Didn't find a supported MDM-AP peripheral");
        return false;
    }

    return true;
}

bool ARMKinetisDebug::reset()
{
    // System resets can be slow, give them more time than the default.
    const unsigned resetRetries = 5000;

    // Put the control register in a known state, and make sure we aren't already in the middle of a reset
    uint32_t status;
    if (!apWrite(REG_MDM_CONTROL, REG_MDM_CONTROL_CORE_HOLD_RESET))
    {
      Serial.println("Setting REG_MDM_CONTROL_CORE_HOLD_RESET failed");
      return false;
    }

    if (!apReadPoll(REG_MDM_STATUS, status, REG_MDM_STATUS_SYS_NRESET, -1, resetRetries))
    {
      Serial.println("Reset status not verified");
      return false;
    }

    // System reset
    if (!apWrite(REG_MDM_CONTROL, REG_MDM_CONTROL_SYS_RESET_REQ))
    {
      Serial.println("Setting REG_MDM_CONTROL_SYS_RESET_REQ failed");
      return false;
    }

    if (!apReadPoll(REG_MDM_STATUS, status, REG_MDM_STATUS_SYS_NRESET, 0))
    {
      Serial.println("Verifing reset flag failed");
      return false;
    }

    if (!apWrite(REG_MDM_CONTROL, 0))
    {
      Serial.println("Resetting control register failed");
      return false;
    }


    // Wait until the flash controller is ready & system is out of reset.
    // Also wait for security bit to be cleared. Early in reset, the chip is determining
    // its security status. When the security bit is set, AHB-AP is disabled.
    if (!apReadPoll(REG_MDM_STATUS, status,
            REG_MDM_STATUS_SYS_NRESET | REG_MDM_STATUS_FLASH_READY | REG_MDM_STATUS_SYS_SECURITY,
            REG_MDM_STATUS_SYS_NRESET | REG_MDM_STATUS_FLASH_READY,
            resetRetries))
            {
              Serial.println("Querying status flag for flash ready failed");
              return false;
            }


    return true;
}

bool ARMKinetisDebug::initK20()
{
    /*
     * ARM peripheral initialization, based on the peripheral startup code
     * used in Teensyduino. We set up the same peripherals that FC-Boot sets up.
     */

    uint8_t value;
    return
        // Enable peripheral clocks
        memStore(REG_SIM_SCGC5, 0x00043F82) && // clocks active to all GPIO
        memStore(REG_SIM_SCGC6,
            REG_SIM_SCGC6_RTC | REG_SIM_SCGC6_FTM0 | REG_SIM_SCGC6_FTM1 |
            REG_SIM_SCGC6_ADC0 | REG_SIM_SCGC6_FTFL) &&

        // Start in FEI mode
        // Enable capacitors for crystal
        memStoreByte(REG_OSC0_CR, REG_OSC_SC8P | REG_OSC_SC2P) &&

        // Enable osc, 8-32 MHz range, low power mode
        memStoreByte(REG_MCG_C2, REG_MCG_C2_RANGE0(2) | REG_MCG_C2_EREFS) &&

        // Switch to crystal as clock source, FLL input = 16 MHz / 512
        memStoreByte(REG_MCG_C1, REG_MCG_C1_CLKS(2) | REG_MCG_C1_FRDIV(4)) &&

        // Wait for crystal oscillator to begin
        memPollByte(REG_MCG_S, value, REG_MCG_S_OSCINIT0, -1) &&

        // Wait for FLL to use oscillator
        memPollByte(REG_MCG_S, value, REG_MCG_S_IREFST, 0) &&

        // Wait for MCGOUT to use oscillator
        memPollByte(REG_MCG_S, value, REG_MCG_S_CLKST_MASK, REG_MCG_S_CLKST(2)) &&

        // Now we're in FBE mode
        // Config PLL input for 16 MHz Crystal / 4 = 4 MHz
        memStoreByte(REG_MCG_C5, REG_MCG_C5_PRDIV0(3)) &&

        // Config PLL for 96 MHz output
        memStoreByte(REG_MCG_C6, REG_MCG_C6_PLLS | REG_MCG_C6_VDIV0(0)) &&

        // Wait for PLL to start using xtal as its input
        memPollByte(REG_MCG_S, value, REG_MCG_S_PLLST, -1) &&

        // Wait for PLL to lock
        memPollByte(REG_MCG_S, value, REG_MCG_S_LOCK0, -1) &&

        // Now we're in PBE mode
        // Config divisors: 48 MHz core, 48 MHz bus, 24 MHz flash
        memStore(REG_SIM_CLKDIV1, REG_SIM_CLKDIV1_OUTDIV1(1) |
            REG_SIM_CLKDIV1_OUTDIV2(1) | REG_SIM_CLKDIV1_OUTDIV4(3)) &&

        // Switch to PLL as clock source, FLL input = 16 MHz / 512
        memStoreByte(REG_MCG_C1, REG_MCG_C1_CLKS(0) | REG_MCG_C1_FRDIV(4)) &&

        // Wait for PLL clock to be used
        memPollByte(REG_MCG_S, value, REG_MCG_S_CLKST_MASK, REG_MCG_S_CLKST(3)) &&

        // Now we're in PEE mode
        // Configure USB for 48 MHz clock
        // USB = 96 MHz PLL / 2
        memStore(REG_SIM_CLKDIV2, REG_SIM_CLKDIV2_USBDIV(1)) &&

        // USB uses PLL clock, trace is CPU clock, CLKOUT=OSCERCLK0
        memStore(REG_SIM_SOPT2, REG_SIM_SOPT2_USBSRC | REG_SIM_SOPT2_PLLFLLSEL |
            REG_SIM_SOPT2_TRACECLKSEL | REG_SIM_SOPT2_CLKOUTSEL(6)) &&

        // Enable USB clock gate
        memStore(REG_SIM_SCGC4, REG_SIM_SCGC4_USBOTG) &&

        // Reset USB core
        memStoreByte(REG_USB0_USBTRC0, REG_USB_USBTRC_USBRESET) &&
        memPollByte(REG_USB0_USBTRC0, value, REG_USB_USBTRC_USBRESET, 0) &&

        // Enable USB
        memStoreByte(REG_USB0_CTL, REG_USB_CTL_USBENSOFEN) &&
        memStoreByte(REG_USB0_USBCTRL, 0) &&

        // USB pull-up off for now
        usbSetPullup(false) &&

        // Test AHB-AP: Can we successfully write to RAM?
        testMemoryAccess();
}

bool ARMKinetisDebug::testMemoryAccess()
{
    // Try word-wide stores to SRAM
    if (!memStoreAndVerify(0x20000000, 0x31415927))
        return false;
    if (!memStoreAndVerify(0x20000000, 0x76543210))
        return false;

    // Test byte-wide memory access
    uint32_t word;
    uint8_t byte;
    if (!memStoreByte(0x20000001, 0x55))
        return false;
    if (!memStoreByte(0x20000002, 0x9F))
        return false;
    if (!memLoad(0x20000000, word))
        return false;
    if (word != 0x769F5510) {
        log(LOG_ERROR, "ARMKinetisDebug: Byte-wide AHB write seems broken! (Test word = %08x)", word);
        return false;
    }
    if (!memLoadByte(0x20000003, byte))
        return false;
    if (byte != 0x76) {
        log(LOG_ERROR, "ARMKinetisDebug: Byte-wide AHB read seems broken! (Test byte = %02x)", byte);
        return false;
    }

    // Test halfword-wide memory access
    uint16_t half;
    if (!memStoreHalf(0x20000000, 0x5abc))
        return false;
    if (!memStoreHalf(0x20000002, 0xdef0))
        return false;
    if (!memLoad(0x20000000, word))
        return false;
    if (word != 0xdef05abc) {
        log(LOG_ERROR, "ARMKinetisDebug: Halfword-wide AHB write seems broken! (Test word = %08x)", word);
        return false;
    }
    if (!memLoadHalf(0x20000002, half))
        return false;
    if (half != 0xdef0) {
        log(LOG_ERROR, "ARMKinetisDebug: Halfword-wide AHB read seems broken! (Test half = %04x)", half);
        return false;
    }

    return true;
}

bool ARMKinetisDebug::flashMassErase()
{
    // Erase all flash, even if some of it is protected.

    uint32_t status;
    if (!apRead(REG_MDM_STATUS, status))
        return false;
    if (!(status & REG_MDM_STATUS_FLASH_READY)) {
        log(LOG_ERROR, "FLASH: Flash controller not ready before mass erase");
        return false;
    }
    if ((status & REG_MDM_STATUS_FLASH_ERASE_ACK)) {
        log(LOG_ERROR, "FLASH: Mass erase already in progress");
        //return false;
    }
    if (!(status & REG_MDM_STATUS_MASS_ERASE_ENABLE)) {
        log(LOG_ERROR, "FLASH: Mass erase is disabled!");
        return false;
    }

    log(LOG_NORMAL, "FLASH: Beginning mass erase operation");
    if (!apWrite(REG_MDM_CONTROL, REG_MDM_CONTROL_CORE_HOLD_RESET | REG_MDM_CONTROL_MASS_ERASE))
        return false;

    // Wait for the mass erase to begin (ACK bit set)
    if (!apReadPoll(REG_MDM_STATUS, status, REG_MDM_STATUS_FLASH_ERASE_ACK, -1)) {
        log(LOG_ERROR, "FLASH: Timed out waiting for mass erase to begin");
        return false;
    }

    // Wait for it to complete (CONTROL bit cleared)
    uint32_t control;
    if (!apReadPoll(REG_MDM_CONTROL, control, REG_MDM_CONTROL_MASS_ERASE, 0, 10000)) {
        log(LOG_ERROR, "FLASH: Timed out waiting for mass erase to complete");
        return false;
    }

    // Check status again
    if (!apRead(REG_MDM_STATUS, status))
        return false;
    if (!(status & REG_MDM_STATUS_FLASH_READY)) {
        log(LOG_ERROR, "FLASH: Flash controller not ready after mass erase");
        return false;
    }

    log(LOG_NORMAL, "FLASH: Mass erase complete");
    return true;

//TODO: Remove?
    ftfl_handleCommandStatus();
}

bool ARMKinetisDebug::flashSectorBufferInit()
{
    // Use FlexRAM as normal RAM, and erase it. Test to make sure it's working.
    return
        ftfl_setFlexRAMFunction(0xFF);/* &&
        memStoreAndVerify(REG_FLEXRAM_BASE, 0x12345678) &&
        memStoreAndVerify(REG_FLEXRAM_BASE, 0xFFFFFFFF) &&
        memStoreAndVerify(REG_FLEXRAM_BASE + kFlashSectorSize - 4, 0xA5559872) &&
        memStoreAndVerify(REG_FLEXRAM_BASE + kFlashSectorSize - 4, 0xFFFFFFFF);*/
}

bool ARMKinetisDebug::flashSectorBufferWrite(uint32_t bufferOffset, const uint32_t *data, unsigned count)
{
    if (bufferOffset & 3) {
        log(LOG_ERROR, "ARMKinetisDebug::flashSectorBufferWrite alignment error");
        return false;
    }
    if (bufferOffset + (count * sizeof *data) > kFlashSectorSize) {
        log(LOG_ERROR, "ARMKinetisDebug::flashSectorBufferWrite overrun");
        return false;
    }

    return memStore(REG_FLEXRAM_BASE + bufferOffset, data, count);
}

bool ARMKinetisDebug::flashSectorProgram(uint32_t address)
{
    if (address & (kFlashSectorSize-1)) {
        log(LOG_ERROR, "ARMKinetisDebug::flashSectorProgram alignment error");
        return false;
    }

    return ftfl_programSection(address, kFlashSectorSize/4);
}

bool ARMKinetisDebug::flashSectorErase(uint32_t address)
{
      return
        ftfl_busyWait() &&
        memStoreByte(REG_FTFL_FCCOB0, 0x09) &&
        memStoreByte(REG_FTFL_FCCOB1, address >> 16) &&
        memStoreByte(REG_FTFL_FCCOB2, address >> 8) &&
        memStoreByte(REG_FTFL_FCCOB3, address) &&
        ftfl_launchCommand() &&
        ftfl_busyWait() &&
        ftfl_handleCommandStatus("FLASH: Error erasing sector! (FSTAT: %08x)");
}

bool ARMKinetisDebug::setProtectionBits(bool protect)
{
/*    if (!ftfl_busyWait())
    {
        log(LOG_ERROR,"Failed to wait for next command slot");
        return false;
    }

    uint8_t data = 0xFF;
    if (protect) data = 0x00;
    if (!memStoreByte(REG_FTFL_FPROT0, data))
    {
        log(LOG_ERROR,"Failed to write protection bits to REG_FTFL_FPROT0");
        return false;
    }

    if (!memStoreByte(REG_FTFL_FPROT1, data))
    {
        log(LOG_ERROR,"Failed to write protection bits to REG_FTFL_FPROT1");
        return false;
    }

    if (!memStoreByte(REG_FTFL_FPROT2, data))
    {
        log(LOG_ERROR,"Failed to write protection bits to REG_FTFL_FPROT2");
        return false;
    }

    if (!memStoreByte(REG_FTFL_FPROT3, data))
    {
        log(LOG_ERROR,"Failed to write protection bits to REG_FTFL_FPROT3");
        return false;
    }

    if (!memStoreByte(REG_FTFL_FEPROT, data))
    {
        log(LOG_ERROR,"Failed to write protection bits to REG_FTFL_FEPROT");
        return false;
    }

    if (!memStoreByte(REG_FTFL_FDPROT, data))
    {
        log(LOG_ERROR,"Failed to write protection bits to REG_FTFL_FPROT3");
        return false;
    }*/

    return
      ftfl_busyWait() &&
      memStoreByte(REG_FTFL_FCCOB0, 0x06) &&
      memStoreByte(REG_FTFL_FCCOB1, 0) &&
      memStoreByte(REG_FTFL_FCCOB2, 4) &&
      memStoreByte(REG_FTFL_FCCOB3, 0xC) &&
      memStoreByte(REG_FTFL_FCCOB4, 0XFF) &&
      memStoreByte(REG_FTFL_FCCOB5, 0XFF) &&
      memStoreByte(REG_FTFL_FCCOB6, 0XFF) &&
      memStoreByte(REG_FTFL_FCCOB7, 0XFE) &&
      ftfl_launchCommand() &&
      ftfl_busyWait() &&
      ftfl_handleCommandStatus("FLASH: Error verifying sector! (FSTAT: %08x)");

    return true;
}

bool ARMKinetisDebug::eraseEverything()
{
    uint32_t status;

    log(LOG_NORMAL, "FLASH: Beginning mass erase operation");
    if (!apWrite(REG_MDM_CONTROL, REG_MDM_CONTROL_MASS_ERASE))
        return false;

    // Wait for the mass erase to begin (ACK bit set)
    if (!apReadPoll(REG_MDM_STATUS, status, 0, -1)) {
        log(LOG_ERROR, "FLASH: Timed out waiting for mass erase to begin");
        return false;
    }

    // Wait for it to complete (CONTROL bit cleared)
    uint32_t control;
    if (!apReadPoll(REG_MDM_CONTROL, control, REG_MDM_CONTROL_MASS_ERASE, 0, 10000)) {
        log(LOG_ERROR, "FLASH: Timed out waiting for mass erase to complete");
        return false;
    }

    return true;
}

bool ARMKinetisDebug::ftfl_busyWait()
{
    const unsigned retries = 1000;
    uint32_t fstat;

    if (!memPoll(REG_FTFL_FSTAT, fstat, REG_FTFL_FSTAT_CCIF, -1)) {
        log(LOG_ERROR, "FLASH: Error waiting for flash controller");
        return false;
    }

    return true;
}

bool ARMKinetisDebug::ftfl_launchCommand()
{
    // Begin a flash memory controller command, and clear any previous error status.
    return
        //memStoreByte(REG_FTFL_FSTAT, REG_FTFL_FSTAT_ACCERR | REG_FTFL_FSTAT_FPVIOL | REG_FTFL_FSTAT_RDCOLERR) &&
        memStoreByte(REG_FTFL_FSTAT, 0x30) &&
        memStoreByte(REG_FTFL_FSTAT, REG_FTFL_FSTAT_CCIF);
}

bool ARMKinetisDebug::ftfl_programPartitionFunction(uint8_t eepromSize, uint8_t partitionCode)
{
	if (!ftfl_busyWait())
	{
		log(LOG_ERROR,"busywait failed in setFlexRAMFunction");
		return false;
	}

	if (!memStoreByte(REG_FTFL_FCCOB0, 0x80))
	{
		log(LOG_ERROR,"Writing byte to REG_FTFL_FCCOB0 (0x80) failed");
		return false;
	}

	if (!memStoreByte(REG_FTFL_FCCOB4, eepromSize))
	{
		log(LOG_ERROR,"Writing byte to REG_FTFL_FCCOB4 (%02x) failed",eepromSize);
		return false;
	}

	if (!memStoreByte(REG_FTFL_FCCOB5, partitionCode))
	{
		log(LOG_ERROR,"Writing byte to REG_FTFL_FCCOB5 (%02x) failed",partitionCode);
		return false;
	}

	if (!ftfl_launchCommand())
	{
		log(LOG_ERROR,"Launch Flash Command failed");
		return false;
	}

	if (!ftfl_busyWait())
	{
		log(LOG_ERROR,"busywait failed in setFlexRAMFunction");
		return false;
	}

	if (!ftfl_handleCommandStatus())
	{
		log(LOG_ERROR,"Handling Command Status failed");
		return false;
	}

	return true;
}

bool ARMKinetisDebug::ftfl_setFlexRAMFunction(uint8_t controlCode)
{
    if (!ftfl_busyWait())
    {
        log(LOG_ERROR,"busywait failed in setFlexRAMFunction");
        return false;
    }

    if (!memStoreByte(REG_FTFL_FCCOB0, 0x81))
    {
        log(LOG_ERROR,"Writing byte to REG_FTFL_FCCOB0 (0x81) failed");
        return false;
    }

    if (!memStoreByte(REG_FTFL_FCCOB1, controlCode))
    {
        log(LOG_ERROR,"Writing byte to REG_FTFL_FCCOB1 (%02x) failed",controlCode);
        return false;
    }

    if (!ftfl_launchCommand())
    {
        log(LOG_ERROR,"Launch Flash Command failed");
        return false;
    }

    if (!ftfl_busyWait())
    {
        log(LOG_ERROR,"busywait failed in setFlexRAMFunction");
        return false;
    }

    if (!ftfl_handleCommandStatus())
    {
        log(LOG_ERROR,"Handling Command Status failed");
        return false;
    }

    //Poll for successful RAM
    const unsigned retries = 1000;
    uint32_t fstat;

	if (controlCode == 0xFF)
	{
		if (!memPoll(REG_FTFL_FCNFG, fstat, REG_FTFL_FCNFG_RAMRDY, retries)) {
			log(LOG_ERROR, "FLASH: Error waiting for flex RAM to be RAMRDY");
			return false;
		}
	}
	else if (controlCode == 0x00)
	{
		if (!memPoll(REG_FTFL_FCNFG, fstat, REG_FTFL_FCNFG_EEERDY, retries)) {
			log(LOG_ERROR, "FLASH: Error waiting for flex RAM to be EEERDY");
			return false;
		}
	}


    return true;
}

bool ARMKinetisDebug::ftfl_programSection(uint32_t address, uint32_t numLWords)
{
    log(LOG_NORMAL,"Programming section at %08x, numWords: %08x",address,numLWords);

    return
        ftfl_busyWait() &&
        memStoreByte(REG_FTFL_FCCOB0, 0x0B) &&
        memStoreByte(REG_FTFL_FCCOB1, address >> 16) &&
        memStoreByte(REG_FTFL_FCCOB2, address >> 8) &&
        memStoreByte(REG_FTFL_FCCOB3, address) &&
        memStoreByte(REG_FTFL_FCCOB4, numLWords >> 8) &&
        memStoreByte(REG_FTFL_FCCOB5, numLWords) &&
        ftfl_launchCommand() &&
        ftfl_busyWait() &&
        ftfl_handleCommandStatus("FLASH: Error verifying sector! (FSTAT: %08x)");
}

bool ARMKinetisDebug::ftfl_programLongword(uint32_t address, const uint8_t* bytes)
{
      return
        ftfl_busyWait() &&
        memStoreByte(REG_FTFL_FCCOB0, 0x06) &&
        memStoreByte(REG_FTFL_FCCOB1, address >> 16) &&
        memStoreByte(REG_FTFL_FCCOB2, address >> 8) &&
        memStoreByte(REG_FTFL_FCCOB3, address) &&
        memStoreByte(REG_FTFL_FCCOB4, *(bytes+(3*sizeof(uint8_t)))) &&
        memStoreByte(REG_FTFL_FCCOB5, *(bytes+(2*sizeof(uint8_t)))) &&
        memStoreByte(REG_FTFL_FCCOB6, *(bytes+(1*sizeof(uint8_t)))) &&
        memStoreByte(REG_FTFL_FCCOB7, *(bytes+(0*sizeof(uint8_t)))) &&
        ftfl_launchCommand() &&
        ftfl_busyWait() &&
        ftfl_handleCommandStatus("FLASH: Error verifying sector! (FSTAT: %08x)");
}

bool ARMKinetisDebug::ftfl_handleCommandStatus(const char *cmdSpecificError)
{
    /*
     * Handle common errors from an FSTAT register value.
     * The indicated "errorMessage" is used for reporting a command-specific
     * error from MGSTAT0. Returns true on success, false on error.
     */

    uint32_t fstat;
    if (!memLoad(REG_FTFL_FSTAT, fstat))
        return false;

    if (fstat & REG_FTFL_FSTAT_RDCOLERR) {
        log(LOG_ERROR, "FLASH: Bus collision error (FSTAT: %08x)", fstat);
        return false;
    }

    if (fstat & (REG_FTFL_FSTAT_FPVIOL)) {
        log(LOG_ERROR, "FLASH: Flash protection violation error (FSTAT: %08x)", fstat);
        return false;
    }

    if (fstat & (REG_FTFL_FSTAT_ACCERR)) {
        log(LOG_ERROR, "FLASH: Address access error (FSTAT: %08x)", fstat);
        return false;
    }

    if (cmdSpecificError && (fstat & REG_FTFL_FSTAT_MGSTAT0)) {
        // Command-specifid error
        log(LOG_ERROR, cmdSpecificError, fstat);
        return false;
    }

    return true;
}

ARMKinetisDebug::Flasher::Flasher(ARMKinetisDebug &target):
target(target)
{
    sectorSizeInBytes = 1024;
	byteAlignment = 8;
}

bool ARMKinetisDebug::Flasher::installFirmware(File *file)
{
    this->file = file;

    target.log(LOG_NORMAL,"Initialize flash process");
    if (!begin()) return false;
    ESP.wdtFeed();

    target.log(LOG_NORMAL,"Begin Flashing");
/*    while (next())
    {
        address += sectorSizeInBytes;
    }*/

	next();
    ESP.wdtFeed();

    target.log(LOG_NORMAL,"End flashing");

/*	if (!target.reset())
	{
		target.log(LOG_ERROR,"Failed to reset device");
		return false;
	}

	if (!target.debugHalt())
	{
		target.log(LOG_ERROR,"Failed to debug halt device");
		return false;
	}*/

	target.log(LOG_NORMAL,"Resetting FlexRAM");
	if (!end()) return false;

    //target.log(LOG_NORMAL,"Setting protection bits");
    //target.setProtectionBits(false);


	target.log(LOG_NORMAL,"Firmware update complete");
    ESP.wdtFeed();

    return true;
}

bool ARMKinetisDebug::dumpSector(uint32_t address)
{
	log(LOG_NORMAL,"Verifying...");
	uint32_t buffer[kFlashSectorSize/4];
	if (!memLoad(address, buffer, kFlashSectorSize/4))
	{
		log(LOG_ERROR,"Failed to read memory block");
		return false;
	}

	for (unsigned i = 0; i < kFlashSectorSize/4; i++) {
		log(LOG_NORMAL,"Longword at %i: %08x",i,buffer[i]);
	}

	return true;
}

bool ARMKinetisDebug::Flasher::begin()
{
/*	// Start with a mass-erase
	target.log(LOG_NORMAL,"Erasing chip");
	if (!target.flashMassErase())
	{
		target.log(LOG_ERROR,"Mass erase failed");
		return false;
	}
*/
    // Use FlexRAM as normal RAM, for buffering flash sectors
    if (!target.flashSectorBufferInit())
    {
        target.log(LOG_ERROR,"Failed to setup FlexRAM");
        return false;
    }

    address = 0;

    return true;
}

bool ARMKinetisDebug::Flasher::end()
{
	target.log(LOG_NORMAL,"Resetting chip");
	if (!target.reset())
	{
		target.log(LOG_ERROR,"Failed to reset device");
		return false;
	}

	target.log(LOG_NORMAL,"Debug halt");
	if (!target.debugHalt())
	{
		target.log(LOG_ERROR,"Failed to debug halt device");
		return false;
	}
/*
	//Partition data flash for EEPROM - values taken from Teensy-Core eeprom.c
	if (!target.ftfl_programPartitionFunction(0x3,0x03))
	{
		target.log(LOG_ERROR,"Failed to parition data flash for EEPROM");
		return false;
	}
*/
	target.log(LOG_NORMAL,"Set FlexRAM to EEPROM mode");
	//Set FlexRAM for EEPROM
	if (!target.ftfl_setFlexRAMFunction(0x00))
	{
		target.log(LOG_ERROR,"Failed to reset FlexRAM to EEPROM");
		return false;
	}

	return true;
}

bool ARMKinetisDebug::Flasher::next()
{
	uint32_t address = 0;
	uint8_t word[4];
	memset(word,0,4);

	uint32_t buffer[kFlashSectorSize/4];
	memset(buffer,0,sizeof(uint32_t)*(kFlashSectorSize/4));
	uint32_t bufferAddress = 0;
	int sector = 0;
    bool rest = true;

	for (address=0;address<file->size();address=address+1)
	{
        ESP.wdtFeed();

		uint8_t byte = file->read();
		if (address > 0 && address % 4 == 0)
		{
			uint32_t data = (word[3] << 24) + (word[2] << 16) + (word[1] << 8) + word[0];
			buffer[bufferAddress] = data;
			//target.log(LOG_NORMAL, "FLASH: Wrote longword to buffer at %08x, 0: %02x, 1: %02x, 2: %02x, 3: %02x, Word: %08x",address,word[0],word[1],word[2],word[3],data);
			bufferAddress++;
            rest = true;

			if (bufferAddress >= kFlashSectorSize/4)
			{
				//Sector size is 2KB, buffer only 1KB, so we have to erase the section every second time
				if (sector % 2 == 0)
				{
					target.log(LOG_NORMAL, "FLASH: flashSectorErase");
					if (!target.flashSectorErase((sector*kFlashSectorSize)+REG_APPLICATION_BASE))
					{
						target.log(LOG_NORMAL, "FLASH: flashSectorErase failed");
						return false;
					}
					else
					{
						target.log(LOG_NORMAL, "Sector %08x erased", (sector*kFlashSectorSize)+REG_APPLICATION_BASE);
					}
				}

				//Write buffer to device
				target.log(LOG_NORMAL,"Writing buffer to FlexRAM");
				if (!target.flashSectorBufferWrite(0,buffer,kFlashSectorSize/4))
				{
					target.log(LOG_ERROR,"Failed to write buffer to FlexRAM");
					return false;
				}

				target.log(LOG_NORMAL, "FLASH: flashSectorProgram: %08x",(sector*kFlashSectorSize)+REG_APPLICATION_BASE);
				if (!target.ftfl_programSection((sector*kFlashSectorSize)+REG_APPLICATION_BASE, sectorSizeInBytes/8))
				{
					target.log(LOG_ERROR,"FLASH: flashSectorProgram failed 20 times in series - giving up.");
					return false;
				}

				memset(buffer,0,sizeof(uint32_t)*(kFlashSectorSize/4));
				bufferAddress = 0;
				sector++;
                rest = false;
			}

			memset(word,0,4);
		}

		//target.log(LOG_NORMAL,"Read byte %02x, position in word %i, Address %i",byte,address%4,address);
		word[address % 4] = byte;

		ESP.wdtFeed();
	}

    target.log(LOG_NORMAL, "Last sector");

    //Sector size is 2KB, buffer only 1KB, so we have to erase the section every second time
    if (sector % 2 == 0)
    {
        target.log(LOG_NORMAL, "FLASH: flashSectorErase");
        if (!target.flashSectorErase((sector*kFlashSectorSize)+REG_APPLICATION_BASE))
        {
            target.log(LOG_NORMAL, "FLASH: flashSectorErase failed");
            return false;
        }
        else
        {
            target.log(LOG_NORMAL, "Sector %08x erased", (sector*kFlashSectorSize)+REG_APPLICATION_BASE);
        }
    }

	//Write buffer to device
	target.log(LOG_NORMAL,"Writing buffer to FlexRAM");
	if (!target.flashSectorBufferWrite(0,buffer,kFlashSectorSize/4))
	{
		target.log(LOG_ERROR,"Failed to write buffer to FlexRAM");
		return false;
	}

	target.log(LOG_NORMAL, "FLASH: flashSectorProgram: %08x",(sector*kFlashSectorSize)+REG_APPLICATION_BASE);
	if (!target.ftfl_programSection((sector*kFlashSectorSize)+REG_APPLICATION_BASE, sectorSizeInBytes/8))
	{
		target.log(LOG_ERROR,"FLASH: flashSectorProgram failed 20 times in series - giving up.");
		return false;
	}

	return false;
}

bool ARMKinetisDebug::Flasher::next2()
{
/*    target.log(LOG_NORMAL, "FLASH: flashSectorErase");
    if (!target.flashSectorErase(address+REG_APPLICATION_BASE))
    {
        target.log(LOG_NORMAL, "FLASH: flashSectorErase failed");
        return false;
    }
    else
    {
        target.log(LOG_NORMAL, "Sector %08x erased", address+REG_APPLICATION_BASE);
    }*/

    bool result = true;
    uint32_t bufferAddress = 0;
    uint8_t word[4];
    uint16_t i = 0;
    memset(word,0,4);
	uint32_t buffer[kFlashSectorSize/4];
	memset(buffer,0,sizeof(uint32_t)*(kFlashSectorSize/4));
    while(bufferAddress < (sectorSizeInBytes/4))
    {
        if (i > 0 && i % 4 == 0)
        {
            //uint32_t data = (word[0] << 24) + (word[1] << 16) + (word[2] << 8) + word[3];
	        uint32_t data = (word[3] << 24) + (word[2] << 16) + (word[1] << 8) + word[0];
/*            if (!target.memStore(bufferAddress + REG_FLEXRAM_BASE,data))
            {
                target.log(LOG_NORMAL, "FLASH: Failed to write longword at %08x, 0: %02x, 1: %02x, 2: %02x, 3: %02x, Word: %08x",bufferAddress,word[0],word[1],word[2],word[3],data);
            }
            else
            {
                target.log(LOG_NORMAL, "FLASH: Wrote longword at %08x, 0: %02x, 1: %02x, 2: %02x, 3: %02x, Word: %08x",bufferAddress,word[0],word[1],word[2],word[3],data);
            }*/

	        if (address == 0 && bufferAddress < 4)
	        {
		        //data = 0xFFFFFFFF;
	        }

	        target.log(LOG_NORMAL, "FLASH: Wrote longword at %08x, 0: %02x, 1: %02x, 2: %02x, 3: %02x, Word: %08x",bufferAddress,word[0],word[1],word[2],word[3],data);
	        buffer[bufferAddress] = data;

            memset(word,0,4);
            bufferAddress++;
        }

        int byte = file->read();
        if (byte == -1)
        {
            result = false;
            break;
        }
        else
        {
/*            if (address+i >=0x400 && address+i <= 0x40F)
            {
                target.log(LOG_NORMAL,"Overriding flash security bit");
                byte = 0xFF;
            }*/
            word[i % 4] = byte;
        }

        ESP.wdtFeed();
        i++;
    }

    if (result == false && i % 4 != 0)
    {
        Serial.println("Packing last word");

	    //uint32_t data = (word[0] << 24) + (word[1] << 16) + (word[2] << 8) + word[3];
	    uint32_t data = (word[3] << 24) + (word[2] << 16) + (word[1] << 8) + word[0];
/*        if (!target.memStore(bufferAddress+REG_FLEXRAM_BASE,data))
        {
            target.log(LOG_NORMAL, "FLASH: Failed to write longword at %08x, 0: %02x, 1: %02x, 2: %02x, 3: %02x, Word: %08x",bufferAddress,word[0],word[1],word[2],word[3],data);
        }
        else
        {
            const uint8_t* bytes = &word[0];
            uint8_t byte0 = *(bytes+(3*sizeof(uint8_t)));
            uint8_t byte1 = *(bytes+(2*sizeof(uint8_t)));
            uint8_t byte2 = *(bytes+(1*sizeof(uint8_t)));
            uint8_t byte3 = *(bytes+(0*sizeof(uint8_t)));
            target.log(LOG_NORMAL, "FLASH: Wrote longword at %08x, 0: %02x, 1: %02x, 2: %02x, 3: %02x, Word: %08x",bufferAddress,byte0,byte1,byte2,byte3,data);
        }*/

	    target.log(LOG_NORMAL, "FLASH: Wrote longword at %08x, 0: %02x, 1: %02x, 2: %02x, 3: %02x, Word: %08x",bufferAddress,word[0],word[1],word[2],word[3],data);

	    buffer[bufferAddress] = data;

	    bufferAddress++;
    }
/*
	//Fill rest of section with 0 bytes
	while(bufferAddress < (sectorSizeInBytes/4))
	{
		uint32_t data = 0;
		if (!target.memStore(bufferAddress + REG_FLEXRAM_BASE,data))
		{
			target.log(LOG_NORMAL, "FLASH: Failed to write longword at %08x, 0: %02x, 1: %02x, 2: %02x, 3: %02x, Word: %08x",bufferAddress,word[0],word[1],word[2],word[3],data);
		}
		else
		{
			target.log(LOG_NORMAL, "FLASH: Wrote longword at %08x, 0: %02x, 1: %02x, 2: %02x, 3: %02x, Word: %08x",bufferAddress,word[0],word[1],word[2],word[3],data);
		}

		bufferAddress++;
	}*/

	//Write buffer to device
	target.log(LOG_NORMAL,"Writing buffer to FlexRAM");
	if (!target.flashSectorBufferWrite(0,buffer,kFlashSectorSize/4))
	{
		target.log(LOG_ERROR,"Failed to write buffer to FlexRAM");
		return false;
	}

    i = 0;
    target.log(LOG_NORMAL, "FLASH: flashSectorProgram: %08x",address+REG_APPLICATION_BASE);
    while (!target.ftfl_programSection(address+REG_APPLICATION_BASE, sectorSizeInBytes/8))
    {
        i++;
        if (i > 20)
        {
            target.log(LOG_ERROR,"FLASH: flashSectorProgram failed 20 times in series - giving up.");
            return false;
        }

        target.log(LOG_NORMAL, "FLASH: flashSectorProgram failed");
        delay(25);
    }

    return result;
}

ARMKinetisDebug::FlashProgrammer::FlashProgrammer(
    ARMKinetisDebug &target, const uint32_t *image, unsigned numSectors)
    : target(target), image(image), numSectors(numSectors)
{}

bool ARMKinetisDebug::FlashProgrammer::begin()
{
    nextSector = 0;
    isVerifying = false;

    // Start with a mass-erase
/*    if (!target.flashMassErase())
        return false;*/

    delay(100);

    // Reset again after mass erase, for new protection bits to take effect
    if (!(target.reset() && target.debugHalt() && target.initK20()))
        return false;

    delay(100);

    // Use FlexRAM as normal RAM, for buffering flash sectors
    if (!target.flashSectorBufferInit())
        return false;

    delay(100);

    return true;
}

bool ARMKinetisDebug::FlashProgrammer::installFirmware(const uint8_t* firmware, uint32_t firmware_length)
{
  //Assert Reset Line
    Serial.println("Resetting Chip");
    //::digitalWrite(4,LOW);

      // Start with a mass-erase
    if (!target.flashMassErase())
        return false;

        delay(200);
    //::digitalWrite(4,HIGH);

    // Reset again after mass erase, for new protection bits to take effect
    if (!(target.reset() && target.debugHalt() && target.initK20()))
        return false;

    uint32_t address = 0;
    for (uint32_t address=0;address<firmware_length;address=address+4)
    {
      if (!target.ftfl_programLongword(address,&firmware[address]))
      {
        target.log(LOG_ERROR, "FLASH: Failed to write longword at %08x",address);
        return false;
      }
      else
      {
        const uint8_t* bytes = &firmware[address];
uint8_t byte0 = *(bytes+(3*sizeof(uint8_t)));
  uint8_t byte1 = *(bytes+(2*sizeof(uint8_t)));
  uint8_t byte2 = *(bytes+(1*sizeof(uint8_t)));
  uint8_t byte3 = *(bytes+(0*sizeof(uint8_t)));

        target.log(LOG_NORMAL, "FLASH: Wrote longword at %08x, 0: %02x, 1: %02x, 2: %02x, 3: %02x, finished: %f",address,byte0,byte1,byte2,byte3,((float)address/(float)firmware_length)*100);
      }
    }

    return true;
}

bool ARMKinetisDebug::FlashProgrammer::installFirmware(File* file)
{
  //Assert Reset Line
    Serial.println("Resetting Chip");
    //::digitalWrite(4,LOW);

      // Start with a mass-erase
    if (!target.flashMassErase())
        return false;

        delay(200);
    //::digitalWrite(4,HIGH);

    // Reset again after mass erase, for new protection bits to take effect
    if (!(target.reset() && target.debugHalt() && target.initK20()))
        return false;

    uint32_t address = 0;
    uint8_t word[4];
    memset(word,0,4);
    for (address=0;address<file->size();address=address+1)
    {
      uint8_t byte = file->read();
      if (address > 0 && address % 4 == 0)
      {
        //word[0] = file->read(word, 4);
        if (!target.ftfl_programLongword(address-4,&word[0]))
        {
          target.log(LOG_ERROR, "FLASH: Failed to write longword at %08x",address-4);
          return false;
        }
        else
        {
          const uint8_t* bytes = &word[0];
          uint8_t byte0 = *(bytes+(3*sizeof(uint8_t)));
          uint8_t byte1 = *(bytes+(2*sizeof(uint8_t)));
          uint8_t byte2 = *(bytes+(1*sizeof(uint8_t)));
          uint8_t byte3 = *(bytes+(0*sizeof(uint8_t)));

	      uint32_t data = (word[3] << 24) + (word[2] << 16) + (word[1] << 8) + word[0];

          target.log(LOG_NORMAL, "FLASH: Wrote longword at %08x, 0: %02x, 1: %02x, 2: %02x, 3: %02x, word: %08x",address-4,byte0,byte1,byte2,byte3,data);
        }

        memset(word,0,4);
      }

      //target.log(LOG_NORMAL,"Read byte %02x, position in word %i, Address %i",byte,address%4,address);
      word[address % 4] = byte;

      ESP.wdtFeed();
    }

    if (address % 4 != 0)
    {
      address -= address % 4;
      Serial.print("Wrote ");
      Serial.print(address);
      Serial.println(" bytes so far, a few left...");

      if (!target.ftfl_programLongword(address,&word[0]))
      {
        target.log(LOG_ERROR, "FLASH: Failed to write longword at %08x",address);
        return false;
      }
      else
      {
        const uint8_t* bytes = &word[0];
        uint8_t byte0 = *(bytes+(3*sizeof(uint8_t)));
        uint8_t byte1 = *(bytes+(2*sizeof(uint8_t)));
        uint8_t byte2 = *(bytes+(1*sizeof(uint8_t)));
        uint8_t byte3 = *(bytes+(0*sizeof(uint8_t)));
        target.log(LOG_NORMAL, "FLASH: Wrote longword at %08x, 0: %02x, 1: %02x, 2: %02x, 3: %02x, finished: %f",address,byte0,byte1,byte2,byte3,((float)address/(float)file->size())*100);
      }
    }

    return true;
}

bool ARMKinetisDebug::FlashProgrammer::isComplete()
{
    return isVerifying && nextSector == numSectors;
}

bool ARMKinetisDebug::FlashProgrammer::next()
{
    uint32_t address = nextSector * kFlashSectorSize;
    const uint32_t *ptr = image + (nextSector * kFlashSectorSize/4);

    if (isVerifying) {
        target.log(LOG_NORMAL, "FLASH: Verifying sector at %08x", address);

        uint32_t buffer[kFlashSectorSize/4];
        if (!target.memLoad(address, buffer, kFlashSectorSize/4))
            return false;

        bool okay = true;

        for (unsigned i = 0; i < kFlashSectorSize/4; i++) {
            if (buffer[i] != ptr[i]) {
                target.log(LOG_ERROR, "FLASH: Verify error at %08x. Expected %08x, actual %08x",
                    address + i*4, ptr[i], buffer[i]);
                okay = false;
            }
        }

        if (!okay)
            return false;

        if (++nextSector == numSectors) {
            // Done with verify!
            target.log(LOG_NORMAL, "FLASH: Programming successful!");
        }

    } else {
        target.log(LOG_NORMAL, "FLASH: Programming sector at %08x", address);

        target.log(LOG_NORMAL, "FLASH: flashSectorErase");
        if (!target.flashSectorErase(address))
        {
          target.log(LOG_NORMAL, "FLASH: flashSectorErase failed");
          return false;
        }
        else
        {
          target.log(LOG_NORMAL, "Sector %08x erased", address);
        }

        target.log(LOG_NORMAL, "FLASH: flashSectorBufferWrite");
        if (!target.flashSectorBufferWrite(0, ptr, kFlashSectorSize/4))
        {
          target.log(LOG_NORMAL, "FLASH: flashSectorBufferWrite failed");
          return false;
        }

        target.log(LOG_NORMAL, "FLASH: flashSectorProgram");
        if (!target.flashSectorProgram(address))
        {
          target.log(LOG_NORMAL, "FLASH: flashSectorProgram failed");
          return false;
        }

        if (++nextSector == numSectors) {
            target.log(LOG_NORMAL, "FLASH: Firmware written to device, resetting and verifying");

            // Done programming. Another reset! Load new protection flags.
            if (!(target.reset() && target.debugHalt()))
                return false;

            nextSector = 0;
            isVerifying = true;
        }
    }

    return true;
}

static inline uint32_t gpioBitBandAddr(uint32_t addr, unsigned bit)
{
    return (addr - 0x40000000) * 32 + bit * 4 + 0x42000000;
}

static inline uint32_t gpioPortAddr(uint32_t base, unsigned p)
{
    return base + (p >> 12) * (REG_GPIOB_PDOR - REG_GPIOA_PDOR);
}

static inline uint32_t gpioPortBit(unsigned p)
{
    return (p >> 2) & 31;
}

bool ARMKinetisDebug::memStoreBit(uint32_t addr, unsigned bit, uint32_t data)
{
    return memStore(gpioBitBandAddr(addr, bit), data);
}

bool ARMKinetisDebug::memLoadBit(uint32_t addr, unsigned bit, uint32_t &data)
{
    return memLoad(gpioBitBandAddr(addr, bit), data);
}

bool ARMKinetisDebug::pinMode(unsigned p, int mode)
{
    // GPIO, and default drive strength + slew rate
    uint32_t pcrValue = REG_PORT_PCR_MUX(1) | REG_PORT_PCR_DSE | REG_PORT_PCR_SRE;

    // PCR address
    uint32_t pcrAddr = REG_PORTA_PCR0 + p;

    switch (mode) {
        case INPUT_PULLUP:
            // Turn on pullup
            pcrValue |= REG_PORT_PCR_PE | REG_PORT_PCR_PS;
            break;

        case INPUT:
        case OUTPUT:
            // Default PCR value
            break;

        default:
            log(LOG_ERROR, "GPIO: Unsupported pinMode %d", mode);
            return true;
    }

    // Set pin mode
    if (!memStore(pcrAddr, pcrValue))
        return false;

    // Set direction
    return memStoreBit(gpioPortAddr(REG_GPIOA_PDDR, p), gpioPortBit(p), mode == OUTPUT);
}

bool ARMKinetisDebug::digitalWrite(unsigned p, int value)
{
    return memStoreBit(gpioPortAddr(REG_GPIOA_PDOR, p), gpioPortBit(p), value != 0);
}

int ARMKinetisDebug::digitalRead(unsigned p)
{
    uint32_t data;
    if (!memLoadBit(gpioPortAddr(REG_GPIOA_PDIR, p), gpioPortBit(p), data))
        return -1;
    return data;
}

bool ARMKinetisDebug::digitalWritePort(unsigned port, unsigned value)
{
    // Write to all bits on a given port
    return memStore(gpioPortAddr(REG_GPIOA_PDOR, port), value);
}

bool ARMKinetisDebug::usbSetPullup(bool enable)
{
    return memStoreByte(REG_USB0_CONTROL, enable ? REG_USB_CONTROL_DPPULLUPNONOTG : 0);
}
