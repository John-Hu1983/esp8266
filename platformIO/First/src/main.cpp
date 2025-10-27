#include "../inc/includes.h"

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Start os");

  // 初始化I2C总线
  init_i2c_object(400000);  
}

void loop()
{
  ESP.wdtFeed(); // 喂狗
}