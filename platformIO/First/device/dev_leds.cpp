#include "dev_leds.h"

void LEDDriver::config(u8 pin)
{
  _pin = pin;
}

void LEDDriver::begin()
{
  pinMode(_pin, OUTPUT);
  turnOff();
}

void LEDDriver::turnOn()
{
  _state = true;
  digitalWrite(_pin, HIGH);
}

void LEDDriver::turnOff()
{
  _state = false;
  digitalWrite(_pin, LOW);
}

void LEDDriver::toggle()
{
  if (_state)
    turnOff();
  else
    turnOn();
}

bool LEDDriver::isOn() const
{
  return _state;
}

//========================================================================
const u8 LED_IO[] = {LED1_Pin, LED2_Pin, LED3_Pin, LED4_Pin};
const u8 LED_COUNT = sizeof(LED_IO) / sizeof(LED_IO[0]);
LEDDriver leds[sizeof(LED_IO) / sizeof(LED_IO[0])];

void init_led_group(void)
{
  for (u8 i = 0; i < LED_COUNT; i++)
  {
    leds[i].config(LED_IO[i]);
    leds[i].begin();
  }
}

LEDDriver *read_led_feature(u8 led_index)
{
  if (led_index < LED_COUNT)
    return &leds[led_index];
  else
    return nullptr;
}

void write_led_level(u8 led_index, bool level)
{
  if (led_index < LED_COUNT)
  {
    if (level)
      leds[led_index].turnOn();
    else
      leds[led_index].turnOff();
  }
}
