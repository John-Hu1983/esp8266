#include "../inc/includes.h"

unsigned long previousMillis = 0;
const unsigned long interval = 100; // 1 second

void setup()
{

  Serial.begin(76800);

  while (!Serial)
  {
    delay(10);
  }

  init_led_group();
  init_os_core();
}

void loop()
{
  unsigned long currentMillis = millis();
  LEDDriver *led;
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    for (u8 i = 0; i < 3; i++)
    {
      led = read_led_feature(i);
      if (led != nullptr)
        led->toggle();
      // char buffer[30]; // 创建一个足够大的缓冲区
      // sprintf(buffer, "LED %d is now ", i);
      // Serial.print(buffer);
      // Serial.println(led->isOn() ? "ON" : "OFF");
    }
  }
}
