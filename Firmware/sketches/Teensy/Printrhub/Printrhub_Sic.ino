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
#include <ILI9341_t3.h>
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>
#include <font_Arial.h>

// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 ctp = Adafruit_FT6206();

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 0
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_SCLK 13

//ILI9341_t3::ILI9341_t3(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi, uint8_t sclk, uint8_t miso)
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

float scrollOffset = 0;
float currentScrollOffset = 0;
int addScrollOffset = 0;
bool touching = false;
TS_Point touchPoint;
float velocity = 0;
TS_Point lastTouchPoint;

void setup(void)
{
 //   while (!Serial);     // used for leonardo debugging

    pinMode(13, OUTPUT);
    digitalWrite(13,LOW);

    Serial.begin(115200);
    Serial.println(F("Printrhub - LCD Controller and Hub for Printrbots!"));

    tft.begin();
    //tft.setRotation(1);

/*    SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
    writecommand_cont(0x33);
    writedata16_last(offset);
    SPI.endTransaction();*/

    if (! ctp.begin(40))
    {  // pass in 'sensitivity' coefficient
        Serial.println("Couldn't start FT6206 touchscreen controller");
        while (1);
    }
    Serial.println("Capacitive touchscreen started");

    tft.fillScreen(ILI9341_BLACK);

    tft.drawRect(5,90,240-5,45,ILI9341_WHITE);
    tft.setCursor(15,103);
    tft.setFont(Arial_18);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_WHITE);
    tft.println("Settings");

    tft.drawRect(5,134,240-5,45,ILI9341_WHITE);
    tft.setCursor(15,103+44);
    tft.setFont(Arial_18);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_WHITE);
    tft.println("Print");

}

void touchBegan(TS_Point p)
{
    Serial.println("Touch began");
    touchPoint = p;
    scrollOffset = currentScrollOffset;
}

void touchMoved(TS_Point p)
{
    Serial.println("Touch moved");
    int diff = p.y - touchPoint.y;
    currentScrollOffset = scrollOffset + diff;

    velocity = p.y - lastTouchPoint.y;
}

void touchEnded()
{
    Serial.println("Touch ended");
}

void loop()
{
    if (ctp.touched())
    {
        TS_Point p = ctp.getPoint();
        if (!touching)
        {
            //Touches began
            touching = true;
            touchBegan(p);
        }
        else
        {
            if (lastTouchPoint.y != p.y)
            {
                touchMoved(p);
            }
        }

        lastTouchPoint = p;
    }
    else
    {
        if (touching)
        {
            //Touches ended
            touching = false;
            touchEnded();
        }
    }

    if (!touching)
    {
        if (velocity > 0)
        {
            velocity -= fabs(velocity) * 0.1;
            if (velocity < 0) velocity = 0;
            Serial.print("Down: ");
            Serial.println(velocity);
            currentScrollOffset += velocity;
        }
        else if (velocity < 0)
        {
            velocity += fabs(velocity) * 0.1;
            if (velocity > 0) velocity = 0;
            Serial.print("Up: ");
            Serial.println(velocity);
            currentScrollOffset += velocity;
        }
    }

    if (currentScrollOffset < 0)
    {
        currentScrollOffset = 319;
    }
    if (currentScrollOffset > 319) currentScrollOffset = 0;

    Serial.print(currentScrollOffset);
    Serial.print(", ");
    Serial.println(velocity);

    tft.setScroll(currentScrollOffset);
    delay(16);
}