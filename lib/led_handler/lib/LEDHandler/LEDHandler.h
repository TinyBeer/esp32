#ifndef LEDHandler_H
#define LEDHandler_H

#include <Arduino.h>
#include "driver/timer.h"

class LEDHandler
{
private:
    int _ledPin;

public:
    LEDHandler(int pin);

    void on();

    void off();
};

#endif