/*
 Printrbot firmware based on Marlin, Sprinter and grbl.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 (https://github.com/MarlinFirmware/Marlin)
 (https://github.com/kliment/Sprinter)
 (https://github.com/simen/grbl/tree)
*/

#include "Marlin.h"

#ifdef ENABLE_AUTO_BED_LEVELING
#include "vector_3.h"
  #ifdef ACCURATE_BED_LEVELING
    #include "qr_solve.h"
  #endif
#endif // ENABLE_AUTO_BED_LEVELING

#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "motion_control.h"
#include "storage.h"
#include "language.h"
#include "pbserial.h"
#include "controller.h"


#if defined(DIGIPOTSS_PIN) && DIGIPOTSS_PIN > -1
#include <SPI.h>
#endif

#define VERSION_STRING  "1.0.0"

// look here for descriptions of gcodes: http://linuxcnc.org/handbook/gcode/g-code.html
// http://objects.reprap.org/wiki/Mendel_User_Manual:_RepRapGCodes

//Implemented Codes
//-------------------
// G0  -> G1
// G1  - Coordinated Movement X Y Z E
// G2  - CW ARC
// G3  - CCW ARC
// G4  - Dwell S<seconds> or P<milliseconds>
// G28 - Home all Axis
// G29 - Detailed Z-Probe, probes the bed at 3 points.  You must de at the home position for this to work correctly.
// G90 - Use Absolute Coordinates
// G91 - Use Relative Coordinates
// G92 - Set current position to cordinates given

// M Codes
// M17  - Enable/Power all stepper motors
// M18  - Disable all stepper motors; same as M84

// M42  - Change pin status via gcode Use M42 Px Sy to set pin x to value y, when omitting Px the onboard led will be used.
// M82  - Set E codes absolute (default)
// M83  - Set E codes relative while in Absolute Coordinates (G90) mode
// M84  - Disable steppers until next move,
//        or use S<seconds> to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
// M85  - Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
// M92  - Set axis_steps_per_unit - same syntax as G92
// M104 - Set extruder target temp
// M105 - Read current temp
// M106 - Fan on
// M107 - Fan off
// M109 - Sxxx Wait for extruder current temp to reach target temp. Waits only when heating
//        Rxxx Wait for extruder current temp to reach target temp. Waits when heating and cooling
// M114 - Output current position to serial port
// M119 - Output Endstop status to serial port
// M140 - Set bed target temp
// M190 - Sxxx Wait for bed current temp to reach target temp. Waits only when heating
//        Rxxx Wait for bed current temp to reach target temp. Waits when heating and cooling
// M201 - Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
// M203 - Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
// M204 - Set default acceleration: S normal moves T filament only moves (M204 S3000 T7000) im mm/sec^2  also sets minimum segment time in ms (B20000) to prevent buffer underruns and M20 minimum feedrate
// M205 -  advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk, E=maximum E jerk
// M206 - set additional homeing offset
// M210 - set travel soft minimum
// M211 - set travel soft maximum
// M212 - Set probe offset for bed leveling
// M213 - Set the maximum homing feedrates in mm/sec (M213 X1000 Y1000 Z500)
// M220 S<factor in percent>- set speed factor override percentage
// M221 S<factor in percent>- set extrude factor override percentage
// M301 - Set PID parameters P I and D
// M302 - Allow cold extrudes, or set the minimum extrude S<temperature>.
// M303 - PID relay autotune S<temperature> sets the target temperature. (default target temperature = 150C)
// M304 - Set bed PID parameters P I and D
// M400 - Finish all moves
// M500 - writes paramters to EEPROM
// M501 - loads parameters from EEPROM (if you need reset them after you changed them temporarily).
// M502 - reverts to the default "factory settings".  You still need to store them in EEPROM afterwards if you want to.
// M907 - Set digital trimpot motor current using axis codes.

// M908 - Control digital trimpot directly.
// M350 - Set microstepping mode.
// M351 - Toggle MS1 MS2 pins directly.
// M928 - Start SD logging (M928 filename.g) - ended by M29
// M999 - Restart after being stopped by error

// ------------- commands
// $info - read printer information (serial, model, bed size...)
// $writeserial - writes serial number ($writeserial 1234-23445-23456-3223445)
// $serial - print serial number value

Controller controller;

unsigned long starttime=0;
unsigned long stoptime=0;


extern "C"{
  extern unsigned int __bss_end;
  extern unsigned int __heap_start;
  extern void *__brkval;

  int freeMemory() {
    int free_memory;

    if((int)__brkval == 0)
      free_memory = ((int)&free_memory) - ((int)&__bss_end);
    else
      free_memory = ((int)&free_memory) - ((int)__brkval);

    return free_memory;
  }
}


void setup()
{
#ifdef DISABLE_JTAG
  // From the AT90USB1286 data sheet:
  // When bit 7 of the MCU Control Register is set to 1 the JTAG interface is disabled.
  // In order to avoid unintentional disabling or enabling of the JTAG interface the
  // application software must write this bit to the desired value twice within four
  // cycles to change its value
  MCUCR = 0x80;
  MCUCR = 0x80;
#endif


  PBSerial::init();

  // Check startup - does nothing if bootloader sets MCUSR to 0
  byte mcu = MCUSR;
  if(mcu & 1) PBSerial::log(MSG_POWERUP);
  if(mcu & 2) PBSerial::log(MSG_EXTERNAL_RESET);
  if(mcu & 4) PBSerial::log(MSG_BROWNOUT_RESET);
  if(mcu & 8) PBSerial::log(MSG_WATCHDOG_RESET);
  if(mcu & 32) PBSerial::log(MSG_SOFTWARE_RESET);
  MCUSR=0;

  Storage::readSettings();
  tp_init();
  plan_init();
  st_init();
}


void loop()
{
  // update command buffers
  controller.update();
  // run commands from buffers if available
  controller.process();
  manage_heater();
  Printer::manageInactivity();
  checkHitEndstops();
}
