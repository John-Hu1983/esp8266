#include "../inc/includes.h"

void setup()
{
  Serial.begin(115200);

  show_info_startup();

  // Disable watchdog initially to prevent issues during initialization
  ESP.wdtDisable();

  // Initialize I2C with recovery if needed
  init_oled_device();
  init_encode_object();

  init_main_menu();

  // Re-enable watchdog after initialization
  ESP.wdtEnable(4000); // 4 second timeout

  // Initialize OS core
  init_os_core();
}

void loop()
{
  os_core_task();
}