/***************************************************
  This is our touchscreen painting example for the Adafruit ILI9341
  captouch shield
  ----> http://www.adafruit.com/products/1947

  Check out the links above for our tutorials and wiring diagrams

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include <SPI.h>       // this is needed for display
#include "PHDisplay.h"
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>
#include <font_Arial.h>
#include "IdleSceneController.h"

// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 Touch = Adafruit_FT6206();

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 0
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_SCLK 13

//ILI9341_t3::ILI9341_t3(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi, uint8_t sclk, uint8_t miso)
PHDisplay Display = PHDisplay(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

//This is used all the time, so keep a global reference instead of building it again every time
IdleSceneController* idleScene;

void setup(void)
{
    //while (!Serial);
    delay(2000);

    Serial.begin(115200);
    Serial.println(F("Printrhub - LCD Controller and Hub for Printrbots!"));

    Display.begin();

    if (! Touch.begin(40))
    {  // pass in 'sensitivity' coefficient
        Serial.println("Couldn't start FT6206 touchscreen controller");
        while (1);
    }
    Serial.println("Capacitive touchscreen started");

    Display.fillScreen(ILI9341_BLACK);

    Serial.println("Starting MainMenu");
    idleScene = new IdleSceneController();
    Application.pushScene(idleScene);
}

void loop()
{
    Application.loop();
    delay(16);
}