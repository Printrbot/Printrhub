//
// Created by Phillip Schuster on 28.07.16.
//

#ifndef TEENSY_HAL_H
#define TEENSY_HAL_H

//#define PRINTRHUB_REV_01    //Uncomment if building the code for Printrhub Rev 0.1
#define PRINTRHUB_REV_02  //Uncomment if building the code for Printrhub Rev 0.2

#ifdef PRINTRHUB_REV_01

//General setup
#define LED_PIN 21

//The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 0
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_SCLK 13
#define TFT_TOUCH_SENSE_PIN 17
#define TFT_BACKLIGHT_PWM 22

//Commstack
#define COMMSTACK_DATAFLOW_PIN 33
#define COMMSTACK_DATALOSS_MARKER_PIN 3
#define COMMSTACK_BAUDRATE 1728000

#endif

#ifdef PRINTRHUB_REV_02

//General setup
#define LED_PIN 21

//The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 0
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_SCLK 13
#define TFT_TOUCH_SENSE_PIN 17
#define TFT_BACKLIGHT_PWM 22

//Commstack
#define COMMSTACK_DATAFLOW_PIN 31
#define COMMSTACK_DATALOSS_MARKER_PIN 3
#define COMMSTACK_BAUDRATE 1728000

#endif

#endif //TEENSY_HAL_H
