#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include <Arduino.h>
#include <user_interface.h>
#define USING_TIM_DIV16 true
#define USING_TIM_DIV256 false
#include <ESP8266TimerInterrupt.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

#include "../inc/usrdef.h"

#include "../device/dev_iic.h"
#include "../device/dev_leds.h"
#include "../device/dev_timer.h"
#include "../device/dev_wifi.h"

#include "../gui/oledfont.h"
#include "../gui/bmp.h"
#include "../gui/oled.h"
#include "../gui/gui.h"
#include "../gui/gui_test.h"
#include "../gui/gui_btn.h"

#include "../kernel/os_core.h"

#endif
