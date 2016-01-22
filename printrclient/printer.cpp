#include "printer.h"
#include "Arduino.h"
#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>
#include "macros.h"


char Printer::cmd_buffer[CMD_BUFF_SIZE][MAX_CMD_SIZE];

uint8_t Printer::cmd_buff_read_idx = 0;
uint8_t Printer::cmd_buff_write_idx = 0;
uint8_t Printer::cmd_buff_len = 0;

File Printer::printFile;

bool Printer::printing = false;
PrintBuffer Printer::printBuffer = PrintBuffer();
bool Printer::sendNext = true;

void Printer::getInfo() {
  Serial.println("$info");
}

void Printer::startPrint(String file) {
  DEBUG("start print :");
  DEBUG(file);
  char fid[8];
  file.toCharArray(fid, 8);

  if (SD.exists(fid)) {
    DEBUG("FILE FOUND:");
    Printer::printFile = SD.open(fid, FILE_READ);
    Printer::printing = true;
  } else {
    DEBUG("FILE NOT FOUND");
    Printer::printing = false;
  }
}

String instr;

void Printer::process() {

  if (!Printer::printing)
    return;

  if (!Printer::sendNext)
    return;

  instr = Printer::printFile.readStringUntil('\n');
  instr.trim();

  if (instr.length() < 1)
    return;

  if (instr[0] != ';') {
    Printer::sendNext = false;
  } else {
    DEBUG("SKIPPING COMMENT LINE");
  }
}
