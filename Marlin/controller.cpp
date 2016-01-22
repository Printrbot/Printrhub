#include "controller.h"
#include "printer.h"
#include <Arduino.h>
#include "pbserial.h"
#include "Configuration.h"
#include "gcode.h"
#include "stepper.h"
#include "storage.h"
#include "language.h"
#include "PString.h"
#include "temperature.h"
#include "motion_control.h"
#ifdef ENABLE_AUTO_BED_LEVELING
#include "autolevel.h"
#endif

//#define CMD_MAP(c,v) this-> c ## v()

const int8_t sensitive_pins[] PROGMEM = SENSITIVE_PINS;

Controller::Controller() {
  this->cmd_buff_len = 0;
  this->cmd_buff_read_idx = 0;
  this->cmd_buff_write_idx = 0;
  this->serPort1 = devBuffer();
  this->serPort2 = devBuffer();
  //this->gcode = new Gcode;
}

void Controller::update() {
  // if there is room in cmd_buffer, process first serial
  if (this->cmd_buff_len < (CMD_BUFF_SIZE - 1)) {
    // keep reading serial while there is data, or while line buffer is not full
    while (Serial.available() && this->serPort1.line_idx < MAX_CMD_SIZE) {
        this->serPort1.in_char = Serial.read();
        this->processChar(this->serPort1);
    }
  }
  // still room available? check second serial
  if (this->cmd_buff_len < (CMD_BUFF_SIZE - 1)) {
    while (Serial1.available() && this->serPort2.line_idx < MAX_CMD_SIZE) {
      this->serPort2.in_char = Serial1.read();
      this->processChar(this->serPort2);
    }
  }
}

void Controller::process() {

  if (this->cmd_buff_len < 1)
    return;

  // check if we received $command or g-code
  if (this->cmd_buffer[this->cmd_buff_read_idx][0] == '$') {
    this->runCommand();
  } else {
    this->gcode.setBlock(this->cmd_buffer[this->cmd_buff_read_idx]);
    if (this->gcode.hasCode('G')) {
      this->runGcode();
    } else if (this->gcode.hasCode('M')) {
      this->runMcode();
    } else if (this->gcode.hasCode('T')) {
      this->runTcode();
    } else {
      PBSerial::error(ERR_UNKNOWN_COMMAND);
    }
  }

  this->cmd_buff_read_idx = (this->cmd_buff_read_idx+1) % CMD_BUFF_SIZE;
  this->cmd_buff_len--;
  // reply on serial
  PBSerial::println(MSG_OK);
}

void Controller::processChar(devBuffer &b) {
  // if new line, or if cmd buffer is full
  if(b.in_char == '\n' || b.in_char == '\r' || b.line_idx >= (MAX_CMD_SIZE - 1)) {

    // ignore comments
    if (b.comment_mode) {
      b.comment_mode = false;
      return;
    }

    // check if line is empty
    if (b.line_idx == 0)
      return;

    // terminate the string
    b.line_buff[b.line_idx] = '\0';
    // reset the line buffer
    b.line_idx = 0;

    // TODO: add line number verification and checksum
    strcpy(this->cmd_buffer[this->cmd_buff_write_idx], b.line_buff);

    this->cmd_buff_write_idx = (this->cmd_buff_write_idx+1) % CMD_BUFF_SIZE;
    this->cmd_buff_len++;
  } else {

    if(b.in_char == ';')
      b.comment_mode = true;

    if(!b.comment_mode)
      b.line_buff[b.line_idx++] = b.in_char;
  }
}

void Controller::runCommand() {
  String line = String(this->cmd_buffer[this->cmd_buff_read_idx]);
  String cmd = line.substring(0, line.indexOf(' '));
  String params = line.substring(cmd.length()+1);

  if (cmd == "$info") {
    PBSerial::printf(F("{\"m\":\"$info\", \"d\": {\"sn\": \"%s\", \"model\": \"%s\", \"ver\": \"%s\"}}\n"), \
      Printer::serial_number, \
      PRINTR_MODEL, \
      FIRMWARE_VERSION);
  }
  else if (cmd == "$serial") {
    PBSerial::printf(F("{\"m\":\"$serial\", \"d\": {\"sn\": \"%s\"}}\n"), \
      Printer::serial_number);
  }
  else if (cmd == "$writeserial") {
    char sn[36];
    params.toCharArray(sn, 36);
    Storage::writeSerial(sn);
    PBSerial::println(MSG_OK);
  }
}

void Controller::runGcode() {
  switch((int) this->gcode.getCodeValue()) {
    case 0:
      this->gcode.parseCoordinates();
      this->prepareRapidMove();
      break;
    case 1:
      this->gcode.parseCoordinates();
      this->prepareLinearMove();
      break;
    case 2:
      this->gcode.parseCoordinates();
      this->prepareArcMove(true);
      break;

    case 3:
      this->gcode.parseCoordinates();
      this->prepareArcMove(false);
      break;

    case 4:
      {
      unsigned long t = 0;
      if (this->gcode.hasCode('P'))
        t = this->gcode.getCodeValue();
      if (this->gcode.hasCode('S'))
        t = this->gcode.getCodeValue() * 1000;
      Printer::dwell(t);
      }
      break;

    case 28:
      Printer::home(this->gcode.hasCode(Printer::axis_codes[X_AXIS]),
                    this->gcode.hasCode(Printer::axis_codes[Y_AXIS]),
                    this->gcode.hasCode(Printer::axis_codes[Z_AXIS]));
      break;
    case 29:
      #ifdef ENABLE_AUTO_BED_LEVELING
      Autolevel::run();
      #endif
      break;
    case 90:
      Printer::relative_mode = false;
      break;
    case 91:
      Printer::relative_mode = true;
      break;
    case 92:
      if (this->gcode.hasCode(Printer::axis_codes[E_AXIS])) {
        // stepper.cpp
        st_synchronize();
      }
      for(int8_t i=0; i < 4; i++) {
        if(this->gcode.hasCode(Printer::axis_codes[i])) {
           if(i == E_AXIS) {
             Printer::current_position[E_AXIS] = this->gcode.getCodeValue();
             Printer::destination[E_AXIS] = Printer::current_position[E_AXIS];
             // planner.cpp
             plan_set_e_position(Printer::current_position[E_AXIS]);
           }
           else {
             Printer::current_position[i] = this->gcode.getCodeValue() + Printer::homing_offset[i];
             Printer::destination[i] = Printer::current_position[i];
             plan_set_position(Printer::current_position[X_AXIS], Printer::current_position[Y_AXIS], Printer::current_position[Z_AXIS], Printer::current_position[E_AXIS]);
           }
        }
      }
      break;
  }
}

void Controller::runMcode()
{
  // validate that m code is in array of valid m codes
  switch((int) this->gcode.getCodeValue()) {
    case 17:
      Printer::enableX();
      Printer::enableY();
      Printer::enableZ();
      Printer::enableE0();
      Printer::enableE1();
      break;

    case 42:
      if (this->gcode.hasCode('S')) {
        int pin_status = this->gcode.getCodeValue();
        int pin_number = LED_PIN;
        if (this->gcode.hasCode('P') && pin_status >= 0 && pin_status <= 255)
          pin_number = this->gcode.getCodeValue();
        for(int8_t i = 0; i < (int8_t)sizeof(sensitive_pins); i++) {
          if (sensitive_pins[i] == pin_number) {
            pin_number = -1;
            break;
          }
        }
        #if defined(FAN_PIN) && FAN_PIN > -1
          if (pin_number == FAN_PIN)
            Printer::fan_speed = pin_status;
        #endif
        if (pin_number > -1) {
          pinMode(pin_number, OUTPUT);
          digitalWrite(pin_number, pin_status);
          analogWrite(pin_number, pin_status);
        }
      }
      break;

    case 82:
      Printer::axis_relative[3] = false;
      break;

    case 83:
      Printer::axis_relative[3] = true;
      break;

    case 84:
      if (this->gcode.hasCode('S')) {
        Printer::stepper_inactive_time = this->gcode.getCodeValue() * 1000;
      } else {
          st_synchronize();
          Printer::disableAllSteppers();
      }
      break;

    case 85:
      if (this->gcode.hasCode('S'))
        Printer::max_inactive_time = this->gcode.getCodeValue() * 1000;
      else
        Printer::max_inactive_time = 0;
      break;

    case 92:
      for (int8_t i=0; i < 4; i++) {
        if (this->gcode.hasCode(Printer::axis_codes[i])) {
          if (i == 3) {
            float value = this->gcode.getCodeValue();
            if(value < 20.0) {
              float factor = axis_steps_per_unit[i] / value;
              // planner.cpp
              max_e_jerk *= factor;
              max_feedrate[i] *= factor;
              axis_steps_per_sqr_second[i] *= factor;
            }
            // planner.cpp
            axis_steps_per_unit[i] = value;
          }
          else {
            axis_steps_per_unit[i] = this->gcode.getCodeValue();
          }
        }
      }
      break;

    case 104:
      if (this->gcode.hasCode('S')) {
        Printer::setHotendTemperature(this->gcode.getCodeValue());
        // temperature.cpp
        setWatch();
      }
      break;

    case 105:
      char e0[8]; PString(e0, 8, degHotend(0));
      char e0t[8]; PString(e0t, 8, degTargetHotend(0));
      char b[8]; PString(b, 8, degBed());
      char bt[8]; PString(bt, 8, degTargetBed());
      #if EXTRUDERS == 1
        PBSerial::printf(F("{\"m\":\"m105\", \"d\": {\"e0\": %s, \"e0t\": %s, \"b\": %s, \"bt\": %s}}\n"), e0, e0t, b, bt);
      #endif
      #if EXTRUDERS == 2
        char e1[8]; PString(e1, 8, degHotend(1));
        char e1t[8]; PString(e1t, 8, degTargetHotend(1));
        PBSerial::printf(F("{\"m\":\"m105\", \"d\": {\"e0\": %s, \"e0t\": %s, \"e1\": %s, \"e1t\": %s, \"b\": %s, \"bt\": %s}}\n"), e0, e0t, e1, e1t, b, bt);
      #endif
      break;

    case 106:
      if (this->gcode.hasCode('S'))
        Printer::fan_speed = constrain(this->gcode.getCodeValue(),0,255);
      else
        Printer::fan_speed = 255;
      break;

    case 107:
      Printer::fan_speed = 0;
      break;

    case 109:
      if (!this->gcode.hasCode('S'))
        return;
      Printer::setHotendTemperatureAndWait(this->gcode.getCodeValue());
      break;


    case 114:
        char x[8]; PString(x, 8, Printer::current_position[0]);
        char y[8]; PString(y, 8, Printer::current_position[1]);
        char z[8]; PString(z, 8, Printer::current_position[2]);
        char e[8]; PString(e, 8, Printer::current_position[3]);
        PBSerial::printf(F("{\"m\":\"m114\", \"d\": {\"x\": %s, \"y\": %s, \"z\": %s,\"e\": %s}}\n"),x,y,z,e);
      break;


    case 119:
      Printer::reportEndstops();
      break;


    case 140:
      if (this->gcode.hasCode('S'))
        setTargetBed(this->gcode.getCodeValue());
      break;


    case 190:
      #if defined(TEMP_BED_PIN) && TEMP_BED_PIN > -1
        if (this->gcode.hasCode('S'))
          Printer::setBedTemperatureAndWait(this->gcode.getCodeValue());
      #endif
      break;

    case 201:
      for (int8_t i=0; i<4; i++) {
        if (this->gcode.hasCode(Printer::axis_codes[i])) {
          max_acceleration_units_per_sq_second[i] = this->gcode.getCodeValue();
        }
      }
      // steps per sq second need to be updated to agree with the units per sq second (as they are what is used in the planner)
      reset_acceleration_rates();
      break;

    case 203:
      for (int8_t i=0; i<4; i++) {
        if (this->gcode.hasCode(Printer::axis_codes[i]))
          max_feedrate[i] = this->gcode.getCodeValue();
      }
      break;

    case 204:
      if (this->gcode.hasCode('S'))
        acceleration = this->gcode.getCodeValue();
      if (this->gcode.hasCode('T'))
        retract_acceleration = this->gcode.getCodeValue();
      break;

    case 205:
      if(this->gcode.hasCode('S'))
        minimumfeedrate = this->gcode.getCodeValue();
      if(this->gcode.hasCode('T'))
        mintravelfeedrate = this->gcode.getCodeValue();
      if(this->gcode.hasCode('B'))
        minsegmenttime = this->gcode.getCodeValue();
      if(this->gcode.hasCode('X'))
        max_xy_jerk = this->gcode.getCodeValue();
      if(this->gcode.hasCode('Z'))
        max_z_jerk = this->gcode.getCodeValue();
      if(this->gcode.hasCode('E'))
        max_e_jerk = this->gcode.getCodeValue();
      break;

    case 206:
      for(int8_t i=0; i < 3; i++) {
        if(this->gcode.hasCode(Printer::axis_codes[i]))
          Printer::homing_offset[i] = this->gcode.getCodeValue();
      }
      break;

    case 210:
      for(int8_t i=0; i<3; i++) {
        if(this->gcode.hasCode(Printer::axis_codes[i]))
          Printer::min_position[i] = this->gcode.getCodeValue();
      }
      break;

    case 211:
      for(int8_t i=0; i<3; i++) {
        if(this->gcode.hasCode(Printer::axis_codes[i]))
          Printer::max_position[i] = this->gcode.getCodeValue();
      }
      break;

    case 212:
      for(int8_t i=0; i < 3; i++) {
        if(this->gcode.hasCode(Printer::axis_codes[i])) {
          Printer::probe_offset[i] = this->gcode.getCodeValue();
        }
      }
      break;

    case 213:
      for (int8_t i=0; i<3; i++) {
        if (this->gcode.hasCode(Printer::axis_codes[i]))
          Printer::homing_feedrate[i] = this->gcode.getCodeValue();
      }
      break;

    case 220:
      if (this->gcode.hasCode('S'))
        Printer::feed_multiply = this->gcode.getCodeValue();
      break;

    case 221:
      if (this->gcode.hasCode('S'))
        Printer::extruder_multiply = this->gcode.getCodeValue();
      break;

    case 301: {
      if (this->gcode.hasCode('P'))
        Kp = this->gcode.getCodeValue();
      if (this->gcode.hasCode('I'))
        Ki = scalePID_i(this->gcode.getCodeValue());
      if (this->gcode.hasCode('D'))
        Kd = scalePID_d(this->gcode.getCodeValue());

      #ifdef PID_ADD_EXTRUSION_RATE
      if(this->gcode.hasCode('C'))
        Kc = this->gcode.getCodeValue();
      #endif
      // temperature.cpp
      updatePID();
      char _kp[8]; PString(_kp, 8, Kp);
      char _ki[8]; PString(_ki, 8, Ki);
      char _kd[8]; PString(_kd, 8, Kd);
      char _kc[8]; PString(_kc, 8, Kc);
      PBSerial::printf(F("{\"m\":\"301\", \"d\": {\"p\": %s, \"i\": %s, \"d\": %s, \"c\": %s}}\n"), _kp, _ki, _kd, _kc);
      }
      break;

    case 302:
      if (this->gcode.hasCode('S')) {
        // planner.cpp
        set_extrude_min_temp(this->gcode.getCodeValue());
      }
      break;

    case 303: {
      float temp = 150.0;
      int e=0;
      int c=5;
      if (this->gcode.hasCode('E'))
        e=this->gcode.getCodeValue();
      if (e<0)
        temp=70;
      if (this->gcode.hasCode('S'))
        temp=this->gcode.getCodeValue();
      if (this->gcode.hasCode('C'))
        c=this->gcode.getCodeValue();
      PID_autotune(temp, e, c);
      }
      break;

    #ifdef PIDTEMPBED
    case 304: {
      if(this->gcode.hasCode('P'))
        bedKp = this->gcode.getCodeValue();
      if(this->gcode.hasCode('I'))
        bedKi = scalePID_i(this->gcode.getCodeValue());
      if(this->gcode.hasCode('D'))
        bedKd = scalePID_d(this->gcode.getCodeValue());
      updatePID();
      char _kp[8]; PString(_kp, 8, bedKp);
      char _ki[8]; PString(_ki, 8, bedKi);
      char _kd[8]; PString(_kd, 8, bedKd);
      PBSerial::printf(F("{\"m\":\"301\", \"d\": {\"p\": %s, \"i\": %s, \"d\": %s}}\n"), _kp, _ki, _kd);
      }
      break;
    #endif //PIDTEMP

    case 400:
      st_synchronize();
      break;

    case 500:
      Storage::writeSettings();
      break;

    case 501:
      Storage::readSettings();
      break;

    case 502:
      Storage::loadDefaults();
      break;

    case 503:
      //Config_PrintSettings();
      break;

    case 907:
      #if defined(DIGIPOTSS_PIN) && DIGIPOTSS_PIN > -1
        for(int i=0;i<4;i++) {
          if(this->gcode.hasCode(Printer::axis_codes[i]))
            digipot_current(i, this->gcode.getCodeValue());
        }
        if(this->gcode.hasCode('B'))
          digipot_current(4, this->gcode.getCodeValue());

        if(this->gcode.hasCode('S')) {
          for(int i=0;i<=4;i++)
            digipot_current(i,this->gcode.getCodeValue());
        }
      #endif
      #ifdef MOTOR_CURRENT_PWM_XY_PIN
        if(this->gcode.hasCode('X'))
          digipot_current(0, this->gcode.getCodeValue());
      #endif
      #ifdef MOTOR_CURRENT_PWM_Z_PIN
        if(this->gcode.hasCode('Z'))
          digipot_current(1, this->gcode.getCodeValue());
      #endif
      #ifdef MOTOR_CURRENT_PWM_E_PIN
        if(this->gcode.hasCode('E'))
          digipot_current(2, this->gcode.getCodeValue());
      #endif
      break;

    case 908: {
      #if defined(DIGIPOTSS_PIN) && DIGIPOTSS_PIN > -1
        uint8_t channel,current;
        if(this->gcode.hasCode('P'))
          channel=this->gcode.getCodeValue();
        if(this->gcode.hasCode('S'))
          current=this->gcode.getCodeValue();
        digitalPotWrite(channel, current);
      #endif
      }
      break;



    case 928:

      break;

    case 999:
      Printer::stopped = false;
      PBSerial::requestResend(this->gcode.current_line);
      break;
  }
}

void Controller::runTcode()
{
  PBSerial::log("processing T code");
}

void Controller::prepareRapidMove()
{
  Printer::clampToSoftEndstops(Printer::destination);
  Printer::last_cmd_time = millis();
  Printer::setMaxFeedrate();
  plan_buffer_line(Printer::destination[X_AXIS],
                   Printer::destination[Y_AXIS],
                   Printer::destination[Z_AXIS],
                   Printer::destination[E_AXIS],
                   Printer::feedrate/60,
                   Printer::active_extruder);
  for(int8_t i=0; i < 4; i++) {
    Printer::current_position[i] = Printer::destination[i];
  }
}

void Controller::prepareLinearMove()
{
  Printer::clampToSoftEndstops(Printer::destination);
  Printer::last_cmd_time = millis();
  plan_buffer_line(Printer::destination[X_AXIS],
                   Printer::destination[Y_AXIS],
                   Printer::destination[Z_AXIS],
                   Printer::destination[E_AXIS],
                   (Printer::feedrate * Printer::feed_multiply)/60/100.0,
                   Printer::active_extruder);

  for(int8_t i=0; i < 4; i++) {
    Printer::current_position[i] = Printer::destination[i];
  }
}

void Controller::prepareArcMove(char isclockwise) {
  float r = hypot(Printer::offset[X_AXIS], Printer::offset[Y_AXIS]); // Compute arc radius for mc_arc

  // Trace the arc
  mc_arc(Printer::current_position,
         Printer::destination,
         Printer::offset,
         X_AXIS,
         Y_AXIS,
         Z_AXIS,
         Printer::feedrate * Printer::feed_multiply/60/100.0,
         r,
         isclockwise,
         Printer::active_extruder);

  // As far as the parser is concerned, the position is now == target. In reality the
  // motion control system might still be processing the action and the real tool position
  // in any intermediate location.
  for(int8_t i=0; i < 4; i++) {
    Printer::current_position[i] = Printer::destination[i];
  }
  Printer::last_cmd_time = millis();
}
