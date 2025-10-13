#include "../inc/includes.h"

volatile uint32_t interruptCount = 0;
os_timer_t os_timer;

IRAM_ATTR void timerCallback(void *pArg)
{
  LEDDriver *led;
  led = read_led_feature(3);
  if (led != nullptr)
    led->toggle();
  interruptCount++;
}

void init_os_core(void)
{
  os_timer_setfn(&os_timer, timerCallback, NULL);
  os_timer_arm(&os_timer, 100, true);
}



