/*
╔══════════╦════════════════════════╦═════════════════╦═══════════════╦═════════════════╦═══════════════╦══════════════════════════╦══════════════════════╗
║ pin name ║ internal configuration ║ physical pin nr ║ Marlin pin nr ║ fastio.h pin nr ║ analog pin nr ║ Printrboard rev. B,C,D&E ║  Printrboard rev. F  ║
╠══════════╬════════════════════════╬═════════════════╬═══════════════╬═════════════════╬═══════════════╬══════════════════════════╬══════════════════════╣
║ PA0      ║ AD0                    ║              51 ║ ​28            ║               0 ║               ║ X-STEP                   ║ X-STEP               ║
║ PA1      ║ AD1                    ║              50 ║ 29            ║               1 ║               ║ X-DIR                    ║ X-DIR                ║
║ PA2      ║ AD2                    ║              49 ║ 30            ║               2 ║               ║ Y-STEP                   ║ Y-STEP               ║
║ PA3      ║ AD3                    ║              48 ║ 31            ║               3 ║               ║ Y-DIR                    ║ Y-DIR                ║
║ PA4      ║ AD4                    ║              47 ║ 32            ║               4 ║               ║ Z-STEP                   ║ Z-STEP               ║
║ PA5      ║ AD5                    ║              46 ║ 33            ║               5 ║               ║ Z-DIR                    ║ Z-DIR                ║
║ PA6      ║ AD6                    ║              45 ║ 34            ║               6 ║               ║ E-STEP                   ║ E-STEP               ║
║ PA7      ║ AD7                    ║              44 ║ 35            ║               7 ║               ║ E-DIR                    ║ E-DIR                ║
║          ║                        ║                 ║               ║                 ║               ║                          ║                      ║
║ PB0      ║ SS/PCINT0              ║              10 ║ 20            ║               8 ║               ║ Y-STOP                   ║ SD:CS                ║
║ PB1      ║ PCINT1/SCLK            ║              11 ║ 21            ║               9 ║               ║ ICSP:SCLK                ║ ICSP:SCLK            ║
║ PB2      ║ PDI/PCINT2/MOSI        ║              12 ║ 22            ║              10 ║               ║ ICSP:MOSI                ║ ICSP:MOSI            ║
║ PB3      ║ PDO/PCINT3/MISO        ║              13 ║ 23            ║              11 ║               ║ ICSP:MISO                ║ ICSP:MISO            ║
║ PB4      ║ PCINT4/OC.2A           ║              14 ║ 24            ║              12 ║               ║ n/c(see note 1)          ║ Y-STOP               ║
║ PB5      ║ PCINT5/OC.1A           ║              15 ║ 25            ║              13 ║               ║ n/c(see note 1)          ║ EXP1:PB5             ║
║ PB6      ║ PCINT6/OC.1B           ║              16 ║ 26            ║              14 ║               ║ SD:CS                    ║ EXP1:PB6             ║
║ PB7      ║ PCINT7/OC.0A/OC.1C     ║              17 ║ 27            ║              15 ║               ║ SD:CD                    ║ SD:CD                ║
║          ║                        ║                 ║               ║                 ║               ║                          ║                      ║
║ PC0      ║ A8                     ║              35 ║ 10            ║              16 ║               ║ EXP2:PC0                 ║ EXP2:PC0             ║
║ PC1      ║ A9                     ║              36 ║ 11            ║              17 ║               ║ EXP2:PC1                 ║ EXP2:PC1             ║
║ PC2      ║ A10                    ║              37 ║ 12            ║              18 ║               ║ EXP2:PC2                 ║ EXP2:PC2             ║
║ PC3      ║ A11/T.3                ║              38 ║ 13            ║              19 ║               ║ E-ENABLE                 ║ E-ENABLE             ║
║ PC4      ║ A12/OC.3C              ║              39 ║ 14            ║              20 ║               ║ HOTBED                   ║ HOTBED               ║
║ PC5      ║ A13/OC.3B              ║              40 ║ 15            ║              21 ║               ║ HOTEND                   ║ HOTEND               ║
║ PC6      ║ A14/OC.3A              ║              41 ║ 16            ║              22 ║               ║ PWM-FAN                  ║ PWM-FAN              ║
║ PC7      ║ A15/IC.3/CLKO          ║              42 ║ 17            ║              23 ║               ║ Z-ENABLE                 ║ Z-ENABLE             ║
║          ║                        ║                 ║               ║                 ║               ║                          ║                      ║
║ PD0      ║ OC0B/SCL/INT0          ║              25 ║ 0             ║              24 ║               ║ EXP1:SCL                 ║ I2C:SCL              ║
║ PD1      ║ OC2B/SDA/INT1          ║              26 ║ 1             ║              25 ║               ║ EXP1:SDA                 ║ I2C:SDA              ║
║ PD2      ║ RXD1/INT2              ║              27 ║ 2             ║              26 ║               ║ EXP1:RX1                 ║ EXP1:RX1             ║
║ PD3      ║ TXD1/INT3              ║              28 ║ 3             ║              27 ║               ║ EXP1:TX1                 ║ EXP1:TX1             ║
║ PD4      ║ ICP1                   ║              29 ║ 4             ║              28 ║               ║ EXP2:PD4                 ║ EXP2:PD4             ║
║ PD5      ║ XCK1                   ║              30 ║ 5             ║              29 ║               ║ EXP2:PD5                 ║ EXP2:PD5             ║
║ PD6      ║ T1                     ║              31 ║ 6             ║              30 ║               ║ EXP2:PD6                 ║ EXP2:PD6             ║
║ PD7      ║ T0                     ║              32 ║ 7             ║              31 ║               ║ EXP2:PD7                 ║ EXP2:PD7             ║
║          ║                        ║                 ║               ║                 ║               ║                          ║                      ║
║ PE0      ║ WR                     ║              33 ║ 8             ║              32 ║               ║ EXP2:PE0                 ║ EXP2:PE0             ║
║ PE1      ║ RD                     ║              34 ║ 9             ║              33 ║               ║ EXP2:PE1                 ║ EXP2:PE1             ║
║ PE2      ║ ALE/HWB                ║              43 ║ 46            ║              34 ║               ║ Hardware boot jumper     ║ Hardware boot jumper ║
║ PE3      ║ IUID                   ║               9 ║ 47            ║              35 ║               ║ X-STOP                   ║ X-STOP               ║
║ PE4      ║ INT4/TOSC1             ║              18 ║ 36            ║              36 ║               ║ Z-STOP                   ║ Z-STOP               ║
║ PE5      ║ INT.5/TOSC2            ║              19 ║ 37            ║              37 ║               ║ E-STOP                   ║ EXP1:PE5             ║
║ PE6      ║ INT.6/AIN.0            ║               1 ║ 18            ║              38 ║               ║ Y-ENABLE                 ║ Y-ENABLE             ║
║ PE7      ║ INT.7/AIN.1/UVcon      ║               2 ║ 19            ║              39 ║               ║ X-ENABLE                 ║ X-ENABLE             ║
║          ║                        ║                 ║               ║                 ║               ║                          ║                      ║
║ PF0      ║ ADC0                   ║              61 ║ 38            ║              40 ║ A0            ║ B-THERM                  ║ B-THERM              ║
║ PF1      ║ ADC1                   ║              60 ║ 39            ║              41 ║ A1            ║ E-THERM                  ║ E-THERM              ║
║ PF2      ║ ADC2                   ║              59 ║ 40            ║              42 ║ A2            ║ EXP1:A2                  ║ EXP1:A2              ║
║ PF3      ║ ADC3                   ║              58 ║ 41            ║              43 ║ A3            ║ EXP1:A3                  ║ EXP1:A3              ║
║ PF4      ║ ADC4/TCK               ║              57 ║ 42            ║              44 ║ A4            ║ EXP1:TCK                 ║ EXP1:TCK             ║
║ PF5      ║ ADC5/TMS               ║              56 ║ 43            ║              45 ║ A5            ║ EXP1:TMS                 ║ EXP1:TMS             ║
║ PF6      ║ ADC6/TDO               ║              55 ║ 44            ║              46 ║ A6            ║ EXP1:TDO                 ║ EXP1:TDO             ║
║ PF7      ║ ADC7/TDI               ║              54 ║ 45            ║              47 ║ A7            ║ EXP1:TDI                 ║ EXP1:TDI             ║
╚══════════╩════════════════════════╩═════════════════╩═══════════════╩═════════════════╩═══════════════╩══════════════════════════╩══════════════════════╝
*/

#ifndef PINS_H
#define PINS_H

#define X_MS1_PIN -1
#define X_MS2_PIN -1
#define Y_MS1_PIN -1
#define Y_MS2_PIN -1
#define Z_MS1_PIN -1
#define Z_MS2_PIN -1
#define E0_MS1_PIN -1
#define E0_MS2_PIN -1
#define E1_MS1_PIN -1
#define E1_MS2_PIN -1
#define DIGIPOTSS_PIN -1

/****************************************************************************************
* Teensylu 0.7 / Printrboard pin assignments (AT90USB1286)
* Requires the Teensyduino software with Teensy++ 2.0 selected in Arduino IDE!
  http://www.pjrc.com/teensy/teensyduino.html
* See http://reprap.org/wiki/Printrboard for more info
****************************************************************************************/
#if MOTHERBOARD == 84
#define KNOWN_BOARD 1
#define AT90USB 1286

#ifndef __AVR_AT90USB1286__
#error Oops!  Make sure you have 'Teensy++ 2.0' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH        true

//Disable JTAG pins so they can be used for the Extrudrboard
#define DISABLE_JTAG       true

#define X_STEP_PIN          0
#define X_DIR_PIN           1
#define X_ENABLE_PIN       39

#define Y_STEP_PIN          2
#define Y_DIR_PIN           3
#define Y_ENABLE_PIN       38

#define Z_STEP_PIN          4
#define Z_DIR_PIN           5
#define Z_ENABLE_PIN       23

#define E0_STEP_PIN         6
#define E0_DIR_PIN          7
#define E0_ENABLE_PIN      19

#define E1_STEP_PIN        24
#define E1_DIR_PIN         25
#define E1_ENABLE_PIN      44

#define E2_STEP_PIN        26
#define E2_DIR_PIN         27
#define E2_ENABLE_PIN      45

#define HEATER_0_PIN       21  // Extruder
#define HEATER_1_PIN       46
#define HEATER_2_PIN       47
#define HEATER_BED_PIN     20  // Bed

#define FAN_PIN        16  // Fan

#define X_MIN_PIN 35
#define X_MAX_PIN -1
#define Y_MIN_PIN -1
#define Y_MAX_PIN 12
#define Z_MIN_PIN 36
#define Z_MAX_PIN -1
//#define E_MIN_PIN 28
//#define E_MAX_PIN 29

#define PBNEO_PIN 6

#define TEMP_0_PIN 1  // Extruder / Analog pin numbering
#define TEMP_BED_PIN 0  // Bed / Analog pin numbering
#define SDSS 20


#define TEMP_1_PIN         2
#define TEMP_2_PIN         3

#define SDPOWER            -1
#define LED_PIN            -1
#define ALARM_PIN          -1

#define DAC_STEPPER_CURRENT
#define DAC_STEPPER_ADDRESS	0
#define DAC_STEPPER_ORDER 	{3,2,1,0}
#define DAC_STEPPER_MAX 	3520
#define DAC_STEPPER_VREF 	1 //internal Vref, gain 1x = 2.048V
#define DAC_STEPPER_GAIN	0

#ifndef SDSUPPORT
// these pins are defined in the SD library if building with SD support
  #define SCK_PIN           9
  #define MISO_PIN         11
  #define MOSI_PIN         10
#endif

#endif  // MOTHERBOARD == 8 (Teensylu) or 81 (Printrboard)

#ifndef KNOWN_BOARD
#error Unknown MOTHERBOARD value in configuration.h
#endif

//List of pins which to ignore when asked to change by gcode, 0 and 1 are RX and TX, do not mess with those!
#define _E0_PINS E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN, HEATER_0_PIN,
#if EXTRUDERS > 1
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN, HEATER_1_PIN,
#else
  #define _E1_PINS
#endif
#if EXTRUDERS > 2
  #define _E2_PINS E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN, HEATER_2_PIN,
#else
  #define _E2_PINS
#endif

#define SENSITIVE_PINS {0, 1, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, \
                        HEATER_BED_PIN, FAN_PIN,                  \
                        _E0_PINS _E1_PINS _E2_PINS             \
                        analogInputToDigitalPin(TEMP_0_PIN), analogInputToDigitalPin(TEMP_1_PIN), analogInputToDigitalPin(TEMP_2_PIN), analogInputToDigitalPin(TEMP_BED_PIN) }
#endif
