#ifndef LEDHandler_H
#define LEDHandler_H

class LEDHandler
{
private:
    int ledPin;

public:
    void setup(int pin);

    void on();

    void off();
};

#endif