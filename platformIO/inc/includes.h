#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include <Arduino.h>
#include <user_interface.h>

// Standard Arduino libraries
#include <EEPROM.h>
#include <Wire.h>

// ESP8266 core libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

// Timer configuration for ESP8266TimerInterrupt library
#define USING_TIM_DIV1 true
#define USING_TIM_DIV16 false
#define USING_TIM_DIV256 false

// Include timer interrupt library after configuration
// #include <ESP8266TimerInterrupt.h>

#include "../inc/usrdef.h"

#include "../png/png_heads.h"


#include "../apps/main_menu.h"
#include "../apps/app_wifi.h"

#include "../device/dev_utility.h"
#include "../device/dev_button.h"
#include "../device/dev_iic.h"

#include "../device/dev_leds.h"
#include "../device/dev_timer.h"
#include "../device/dev_encoder.h"
#include "../device/dev_wifi.h"


#include "../gui/oledfont.h"
#include "../gui/bmp.h"
#include "../gui/oled.h"
#include "../gui/gui.h"
#include "../gui/gui_test.h"
#include "../gui/gui_btn.h"

#include "../kernel/os_core.h"

#endif