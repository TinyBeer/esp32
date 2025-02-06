#include <Arduino.h>
#include <WiFi.h>

// key
const int keyPin = 0;
int buttonState = 0;
int lastButtonState = 0;

void initKey()
{
  pinMode(keyPin, INPUT);
}

bool scanKey()
{
  bool pressed = false;
  buttonState = digitalRead(keyPin);
  if (buttonState != lastButtonState)
  {
    if (buttonState == LOW)
    {
      pressed = true;
    }
  }
  lastButtonState = buttonState;
  return pressed;
}

// led pin:
const int ledPin = 2;
int ledChannel = 0;
int brightness = 0;
int fadeDelay = 10;

// breathing light init
// timer
hw_timer_t *timer = NULL;
const int timerPeriod = 5000;
void IRAM_ATTR onTimer()
{
  static double phase = 0;
  phase += 0.01;
  if (phase >= 2 * PI)
  {
    phase = 0;
  }
  brightness = (sin(phase) + 1) * 75.5;
  ledcWrite(ledChannel, brightness);
}

void init_breathing_light(int pin, int channel)
{
  pinMode(ledPin, OUTPUT);
  ledcSetup(ledChannel, 5000, 8);
  ledcAttachPin(ledPin, 0);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, timerPeriod, true);
  timerAlarmEnable(timer);
}

void setup()
{
  initKey();
  Serial.begin(115200);
  init_breathing_light(ledPin, ledChannel);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}
std::string *scanWiFi(int &count)
{
  int netWorkCount = WiFi.scanNetworks();
  std::string *ssids = new std::string[netWorkCount];
  if (netWorkCount > 0)
  {
    count = netWorkCount;
    for (int i = 0; i < netWorkCount; i++)
    {
      ssids[i] = WiFi.SSID(i).c_str();
    }
  }
  return ssids;
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (scanKey())
  {
    Serial.println("scan...");
    int n;
    std::string *ssids = scanWiFi(n);
    for (int i = 0; i < n; i++)
    {
      Serial.println(ssids[i].c_str());
    }
  }
  delay(10);
}