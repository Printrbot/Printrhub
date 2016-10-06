//
// Created by Phillip Schuster on 01.08.16.
//

#ifndef TEENSY_LED_H
#define TEENSY_LED_H

#include "Arduino.h"

enum LEDMode : uint8_t {
    LED_Off = 0,
    LED_On = 1,
    LED_Pulse = 2
};

class LED
{
public:
    LED(uint8_t pin);
    ~LED();

    void begin();
    void end();
    void setMode(LEDMode mode) { _mode = mode; };

    void on();
    void off();
    void blink() { pulse(0.5,true); };
    void pulse(float seconds, bool repeat);

    void loop();

private:
    void update();

private:
    uint8_t _pin;
    LEDMode _mode;
    unsigned long _pulseStart;
    float _pulseDuration;
    bool _pulseRepeat;
};


#endif //TEENSY_LED_H
