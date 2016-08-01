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
#include "framework/core/PHDisplay.h"
#include <Wire.h>      // this is needed for FT6206
#include <SoftwareSerial.h>
#include "drivers/Adafruit_FT6206/Adafruit_FT6206.h"
#include "IdleSceneController.h"
#include "MainSceneController.h"
#include "drivers/SD/SD_t3.h"
#include "ChoosePrintSceneController.h"
#include "LoadFilamentSceneController.h"
#include "SettingsSceneController.h"
#include "PrintStatusSceneController.h"
#include "Bitmaps.h"
#include "framework/core/HAL.h"

// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 Touch = Adafruit_FT6206();

//ILI9341_t3::ILI9341_t3(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi, uint8_t sclk, uint8_t miso)
PHDisplay Display = PHDisplay(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

//This is used all the time, so keep a global reference instead of building it again every time
SceneController* mainController;

#ifdef DEBUG_USE_SOFTWARE_SERIAL
SoftwareSerial DebugSerial(DEBUG_SOFTWARE_SERIAL_RX_PIN,DEBUG_SOFTWARE_SERIAL_TX_PIN);
#endif

LED StatusLED(LED_PIN);

int globalLayerId = 0;

int globalLayersCreated = 0;
int globalLayersDeleted = 0;

int globR = 0;

void testImage(void)
{
    Display.fillScreen(ILI9341_WHITE);

/*    Display.drawMaskedBitmap(10,10,64,64,imageOfArrowIcon_64_64,0,0,64,64,ILI9341_BLACK,ILI9341_WHITE);
    Display.drawMaskedBitmap(10,80,32,30,imageOfCancelPrintIcon_32_30,0,0,32,30,ILI9341_BLACK,ILI9341_WHITE);
    Display.drawMaskedBitmap(10,120,24,24,imageOfWrenchIcon_24_24,0,0,24,24,ILI9341_BLACK,ILI9341_WHITE);*/

    Display.drawBitmap(0,0,320,240,idlescreen,0,0,320,240);

    while(true) {};
}

void showSplashScreen()
{
    Display.drawBitmap(0,0,320,240,idlescreen,0,0,320,240);

    Display.fadeIn();

    delay(2000);

    Display.fadeOut();
}

void setup(void)
{
    //while (!Serial);
    delay(2000);

    //Initiate Status LED to be able to show some errors
    StatusLED.begin();

    StatusLED.pulse(1,false);

#ifdef DebugSerial
    {
        DebugSerial.begin(115200);
    }
#endif

    LOG("Printrhub - LCD Controller and Hub for Printrbots!");

    //Initiate communication to Printerboard
    Serial1.begin(115200);

    //Initiate communication pipeline to ESP8266
    Serial3.begin(COMMSTACK_BAUDRATE);
    //Serial3.attachCts(14);
    //Serial3.attachRts(2);

    //Pull backlight pin to turn on display backlight
    pinMode(TFT_BACKLIGHT_PWM,OUTPUT);
    //digitalWrite(TFT_BACKLIGHT_PWM,HIGH);
    analogWrite(TFT_BACKLIGHT_PWM,0);

    delay(100);

//    pinMode(7,INPUT);
//    pinMode(8,INPUT);

    Display.begin();
    //Rotate to landscape
    Display.setRotation(ILI9341_ORIENTATION_LANDSCAPE_LEFT);
    Display.setScroll(0);

    if (!SD.begin(15))
    {
        Display.fillRect(0,0,320,240,ILI9341_WHITE);
        Display.setCursor(10,10);
        Display.setTextColor(ILI9341_BLACK);
        Display.println("Couldn't start SD card!");
        Display.fadeIn();

        LOG("Couldn't start SD card");
        StatusLED.on();
        while (1);
    }
    LOG("Started SD card interface");

    showSplashScreen();

    if (! Touch.begin(TFT_TOUCH_SENSE_PIN,40))
    {  // pass in 'sensitivity' coefficient
        LOG("Couldn't start FT6206 touchscreen controller");
        StatusLED.on();
        while (1);
    }
    LOG("Capacitive touchscreen started");

    Display.fillScreen(ILI9341_BLACK);

    //testFont();

    //Display.setScrollArea(0,270,50);
    //Display.setScrollInsets(50,0);

    //Initiate Application
    Application.setup();

    LOG("Starting MainMenu");
    //mainController = new MainSceneController();
    //Application.pushScene(mainController);
    //idleScene = new IdleSceneController();
    //Application.pushScene(idleScene);
    //wifiController = new WiFiSetupSceneController();
    //Application.pushScene(wifiController);
    
    mainController = new ChoosePrintSceneController();
    Application.pushScene(mainController);
}

void loop()
{
    Application.loop();
//    delay(16);
}
