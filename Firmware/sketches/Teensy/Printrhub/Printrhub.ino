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
#include "IdleSceneController.h"
#include "MainSceneController.h"
#include "SD_t3.h"

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
MainSceneController* mainController;

int globalLayerId = 0;

int globalLayersCreated = 0;
int globalLayersDeleted = 0;

int globR = 0;

void testImage(void)
{
    File file = SD.open("/feet.dat",FILE_READ);

/*    for (int x=0;x<180;x++)
    {
        uint16_t buffer[151];
        file.read(buffer,sizeof(uint16_t)*150);

        for (int y=0;y<150;y++)
        {
            Display.drawPixel(x,y,buffer[y]);
        }
    }*/

/*    Display.fillScreen(ILI9341_BLACK);

    uint16_t buffer[14800];


    for (int y=0;y<80;y++)
    {
        file.read(buffer,180*sizeof(uint16_t));
        for (int x=0;x<180;x++)
        {
            Display.drawPixel(x+10,y+10,buffer[x]);
        }
    }

    while(true);
*/
    while(true)
    {
        Display.drawFileBitmapByColumn(10,10,180,150,&file,0,0,180,150,1);
        delay(100);
    }
    delay(2000);
    while(true);
}

void setup(void)
{
    //while (!Serial);
    delay(2000);

    Serial.begin(115200);
    Serial.println(F("Printrhub - LCD Controller and Hub for Printrbots!"));

    Serial1.begin(115200);

    //Pull backlight pin to turn on display backlight
    pinMode(TFT_BACKLIGHT_PWM,OUTPUT);
    digitalWrite(TFT_BACKLIGHT_PWM,HIGH);

    Display.begin();
    //Rotate to landscape
    Display.setRotation(ILI9341_ORIENTATION_LANDSCAPE_LEFT);
    Display.setScroll(0);

    if (!SD.begin(15))
    {
        Serial.println("Couldn't start SD card");
        while (1);
    }
    Serial.println("Started SD card interface");

    //testImage();

    if (! Touch.begin(40))
    {  // pass in 'sensitivity' coefficient
        Serial.println("Couldn't start FT6206 touchscreen controller");
        while (1);
    }
    Serial.println("Capacitive touchscreen started");

    Display.fillScreen(ILI9341_BLACK);

    Serial.println("Starting MainMenu");
    mainController = new MainSceneController();
    Application.pushScene(mainController);
    //idleScene = new IdleSceneController();
    //Application.pushScene(idleScene);
    //wifiController = new WiFiSetupSceneController();
    //Application.pushScene(wifiController);

    //ChoosePrintSceneController* choosePrintSceneController = new ChoosePrintSceneController();
    //Application.pushScene(choosePrintSceneController);
}

void loop()
{
    Application.loop();
//    delay(16);
}