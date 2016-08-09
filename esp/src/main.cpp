#include <SPI.h>

#include <EEPROM.h>
#include <ESP8266WiFi.h>
//#include "config.h"
//#include "net.h"
//#include "controller.h"
//#include "printer.h"
#include <Wire.h>
//#include "macros.h"
#include "application.h"
//#include "brain.h"
//#include <EEPROM.h>
//#include "web_server.h"
#include "core/CommStack.h"
//#include "arm_kinetis_debug.h"
//#include "event_logger.h"
#include "core/Mode.h"
//#include "actions/Idle.h"
//#include <GDBStub.h>


extern "C" {
  #include "user_interface.h"
}

//Net net;
//WebServer web;


void setup() {
  system_update_cpu_freq(160);
  //Mode* initialMode = new Idle();
  Application.setup();



/*
  pinMode(13,OUTPUT);
	digitalWrite(13,HIGH);

  Serial.begin(115200*15);
  Serial.println("");

  // check if there are any credentials in eeprom
  EEPROM.begin(512);
  Config::load();
*/
/*
  if (Config::data.blank) {
    // start adhoc server since no data was set
    net.startAdHoc();
  } else {
    if (net.connectWifi()) {
      // connected
      // TODO: check if new firmware is available
      // TODO: notify brain that local wifi is connected
    } else {
      net.startAdHoc();
      // TODO: notify brain that AP is active
    }
  }
  */
  //SPIFFS.begin();
  // star the web server
  //web.begin();
}


void loop() {
  Application.loop();
}
