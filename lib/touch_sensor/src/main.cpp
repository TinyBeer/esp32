#include "TouchSensor.h"

TouchSensor touchSensor;
void setup()
{
  Serial.begin(115200);
}

void loop()
{
  switch (touchSensor.check())
  {
  case TouchSensor::SingleClick:
    Serial.println("Single click...");
    break;
  case TouchSensor::DoubleClick:
    Serial.println("Double click...");
    break;

  default:
    break;
  }
}