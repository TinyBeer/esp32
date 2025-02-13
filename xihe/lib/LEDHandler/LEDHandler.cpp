#include "LEDHandler.h"

LEDHandler::LEDHandler(int pin)
{
    _ledPin = pin;
    pinMode(_ledPin, OUTPUT);
}

void LEDHandler::on()
{
    digitalWrite(_ledPin, HIGH);
}

void LEDHandler::off()
{
    digitalWrite(_ledPin, LOW);
}
