//
// Created by Phillip Schuster on 28.07.16.
//

#ifndef ESP8266_ARM_SWD_HAL_H
#define ESP8266_ARM_SWD_HAL_H

//#define PRINTRHUB_REV_01    //Uncomment if building the code for Printrhub Rev 0.1
#define PRINTRHUB_REV_02  //Uncomment if building the code for Printrhub Rev 0.2

#ifdef PRINTRHUB_REV_01

//Firmware updates for MK20
#define MK20_RESET_PIN 13
#define MK20_SWD_CLK 5
#define MK20_SWD_IO 14

//Commstack
#define COMMSTACK_DATAFLOW_PIN 12
#define COMMSTACK_BAUDRATE 1728000
#define COMMSTACK_WORKING_MARKER_PIN 13
#define COMMSTACK_INFO_MARKER_PIN 12

#endif

#ifdef PRINTRHUB_REV_02

//Firmware updates for MK20
#define MK20_RESET_PIN 13
#define MK20_SWD_CLK 5
#define MK20_SWD_IO 14

//Commstack
#define COMMSTACK_DATAFLOW_PIN 4
#define COMMSTACK_BAUDRATE 1728000
#define COMMSTACK_WORKING_MARKER_PIN 16
#define COMMSTACK_INFO_MARKER_PIN 12

#endif

#endif //ESP8266_ARM_SWD_HAL_H
