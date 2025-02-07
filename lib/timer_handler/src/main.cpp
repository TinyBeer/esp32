#include "TimerHandler.h"

TimerHandler timer(0, []()
                   { digitalWrite(2, !digitalRead(2)); }, 1000);
void setup()
{
  pinMode(2, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
}
