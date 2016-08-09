//
// Created by Phillip Schuster on 01.08.16.
//

#include "LED.h"

LED::LED(uint8_t pin)
{
    _pin = pin;
    _mode = LED_Off;
}

LED::~LED()
{
    end();
}

void LED::begin()
{
    pinMode(_pin,OUTPUT);
    digitalWrite(_pin,LOW);
}

void LED::end()
{
    pinMode(_pin, INPUT);
}

void LED::on()
{
    _mode = LED_On;
    update();
}

void LED::off()
{
    _mode = LED_Off;
    update();
}

void LED::pulse(float seconds, bool repeat)
{
    _mode = LED_Pulse;
    _pulseStart = 0;
    _pulseDuration = seconds;
    _pulseRepeat = repeat;
    update();
}

void LED::update()
{
    float deltaTime = 0;
    unsigned long currentTime = millis();

    if (_mode == LED_On)
    {
        digitalWrite(_pin,HIGH);
    }
    else if (_mode == LED_Off)
    {
        digitalWrite(_pin,LOW);
    }
    else if (_mode == LED_Pulse)
    {
        if (_pulseStart == 0)
        {
            digitalWrite(_pin,HIGH);
            _pulseStart = currentTime;
        }
        else
        {
            deltaTime = (float)(currentTime - _pulseStart)/1000.0f;
            if (deltaTime > _pulseDuration)
            {
                _pulseStart = currentTime;
                digitalWrite(_pin,!digitalRead(_pin));

                if (!_pulseRepeat)
                {
                    _mode = LED_Off;
                }
            }
        }
    }
}

void LED::loop()
{
    update();
}