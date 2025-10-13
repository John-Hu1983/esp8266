#ifndef DEV_LEDS_H
#define DEV_LEDS_H

#include <Arduino.h>

#define LED1_Pin (4)  // 0 on ; 1 off
#define LED2_Pin (14) // 1 on ; 0 off
#define LED3_Pin (12) // 0 on ; 1 off
#define LED4_Pin (5)  // 0 on ; 1 off

class LEDDriver
{
public:
  void config(u8 pin);
  void begin();
  void turnOn();
  void turnOff();
  void toggle();
  bool isOn() const;

private:
  uint8_t _pin;
  bool _state;
};

void init_led_group(void);
LEDDriver *read_led_feature(u8 led_index);
void write_led_level(u8 led_index, bool level);
#endif
