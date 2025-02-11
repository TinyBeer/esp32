#include "ServoHandler.h"

ServoHandler::ServoHandler(int pin)
{
    _servo = Servo();
    _servo.attach(pin);
}

void ServoHandler::write(int angle)
{
    _servo.write(angle);
}