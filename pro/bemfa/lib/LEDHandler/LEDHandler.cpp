#include "LEDHandler.h"
#include <Arduino.h>

void LEDHandler::setup(int pin)
{
    ledPin = pin;
    pinMode(ledPin, OUTPUT);
}

void LEDHandler::on()
{
    digitalWrite(ledPin, HIGH);
}

void LEDHandler::off()
{
    digitalWrite(ledPin, LOW);
}