#ifndef SERVOHANDLER_H
#define SERVOHANDLER_H

#include <ESP32Servo.h>

class ServoHandler
{
public:
    ServoHandler(int pin);

    void write(int angle);

private:
    Servo _servo;
};

#endif