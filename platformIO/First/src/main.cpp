#include "../inc/includes.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>

dev_tim_t led_tim;

// 声明WiFi相关函数
void scanWiFiNetworks();
bool connectToWiFi();

unsigned long previousMillis = 0;
const unsigned long interval = 100;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    delay(10);
  }

  // 初始化LED和OS核心
  // init_led_group(); // 根据需要启用
  // init_os_core();   // 根据需要启用

  // 初始化WiFi
  scanWiFiNetworks();
  connectToWiFi();
}

void loop()
{
  // 检查WiFi连接状态
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi连接已断开，尝试重新连接...");
    connectToWiFi();
  }

  // 原有LED闪烁代码
  unsigned long currentMillis = millis();
  /*
  LEDDriver *led;
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    for (u8 i = 0; i < 3; i++) {
      led = read_led_feature(i);
      if (led != nullptr)
        led->toggle();
    }
  }
  */
}
