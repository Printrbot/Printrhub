/*
 * Hardware abstraction layer. Configures various pins for different revisions
 * of the hardware. First Shipped revision is 04.
 *
 * Copyright (c) 2016 Printrbot Inc.
 *
 * Developed in cooperation with Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
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
#ifndef ESP8266_ARM_SWD_HAL_H
#define ESP8266_ARM_SWD_HAL_H

//#define PRINTRHUB_REV_01    //Uncomment if building the code for Printrhub Rev 0.1
//#define PRINTRHUB_REV_02  //Uncomment if building the code for Printrhub Rev 0.2
#define PRINTRHUB_REV_04  //Uncomment if building the code for Printrhub Rev 0.4

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
#define COMMSTACK_BAUDRATE 115200
#define COMMSTACK_WORKING_MARKER_PIN 16
#define COMMSTACK_INFO_MARKER_PIN 12

#endif

#ifdef PRINTRHUB_REV_04

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
