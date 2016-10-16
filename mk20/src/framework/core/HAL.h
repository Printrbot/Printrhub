//
// Created by Phillip Schuster on 28.07.16.
//

#ifndef TEENSY_HAL_H
#define TEENSY_HAL_H

//#define PRINTRHUB_REV_01    //Uncomment if building the code for Printrhub Rev 0.1
//#define PRINTRHUB_REV_02  //Uncomment if building the code for Printrhub Rev 0.2
//#define PRINTRHUB_REV_03    //Uncomment if building the code for Printrhub Rev 0.3
#define PRINTRHUB_REV_04    //Uncomment if building the code for Printrhub Rev 0.3

#ifdef PRINTRHUB_REV_01

#define CTS_PIN 20
#define RTS_PIN 6

//General setup
#define LED_PIN 21

//Connection to ESP
#define ESP_RESET 5

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

#define G2_RESET_PIN 27
#define G2_ERASE_PIN 28

//Debug
#define DebugSerial Serial

#endif

#ifdef PRINTRHUB_REV_02

//General setup
#define LED_PIN 21

//Connection to ESP
#define ESP_RESET 5

#define CTS_PIN 20
#define RTS_PIN 6


//The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 0
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_SCLK 13
#define TFT_TOUCH_SENSE_PIN 17
#define TFT_BACKLIGHT_PWM 22

#define G2_RESET_PIN 27
#define G2_ERASE_PIN 28

//Commstack
#define COMMSTACK_DATAFLOW_PIN 31
#define COMMSTACK_DATALOSS_MARKER_PIN 3
#define COMMSTACK_BAUDRATE 115200

//Debug
#define DEBUG_USE_SOFTWARE_SERIAL
#define DEBUG_SOFTWARE_SERIAL_RX_PIN 14
#define DEBUG_SOFTWARE_SERIAL_TX_PIN 2
#define DebugSerial DebugSerial

#endif

#ifdef PRINTRHUB_REV_03

//General setup
#define LED_PIN 21

//Connection to ESP
#define ESP_RESET 5

//The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 0
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_SCLK 13
#define TFT_TOUCH_SENSE_PIN 17
#define TFT_BACKLIGHT_PWM 22
#define TFT_RESET 23

//tinyg
#define G2_RESET_PIN 27
#define G2_ERASE_PIN 28

#define CTS_PIN 20
#define RTS_PIN 6

//Commstack
#define COMMSTACK_DATAFLOW_PIN 31
#define COMMSTACK_DATALOSS_MARKER_PIN 3
#define COMMSTACK_BAUDRATE 1728000

//Debug
#define DEBUG_USE_SOFTWARE_SERIAL
#define DEBUG_SOFTWARE_SERIAL_RX_PIN 14
#define DEBUG_SOFTWARE_SERIAL_TX_PIN 2
#define DebugSerial DebugSerial

#endif

#ifdef PRINTRHUB_REV_04

#define CTS_PIN 20
#define RTS_PIN 6

//General setup
#define LED_PIN 21

//Connection to ESP
#define ESP_RESET 5

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
#define COMMSTACK_DATAFLOW_PIN 24
#define COMMSTACK_DATALOSS_MARKER_PIN 2
#define COMMSTACK_BAUDRATE 1728000

#define PRINTER_ACTIVE 3

#define CODE_INDICATOR_1 29
#define CODE_INDICATOR_2 30

#define G2_RESET_PIN 27
#define G2_ERASE_PIN 28

//Debug
#define DebugSerial Serial2
#define DEBUG_SETUP CORE_PIN9_CONFIG = 0; CORE_PIN10_CONFIG = 0; CORE_PIN26_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_PFE | PORT_PCR_MUX(3); CORE_PIN31_CONFIG = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(3);

#endif

#endif //TEENSY_HAL_H
