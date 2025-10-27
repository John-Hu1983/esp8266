#include "../inc/includes.h"

/**
 * @brief Show startup information
 *
 * This function displays the restart reason and exception cause
 * when the ESP8266 module is restarted.
 */
void show_info_startup(void)
{
  // Detect restart reason
  rst_info *resetInfo = ESP.getResetInfoPtr();
  Serial.println("=== System Restart Detection ===");
  Serial.print("Restart reason: ");
  Serial.println(ESP.getResetReason());
  Serial.print("Exception cause: ");
  Serial.println(resetInfo->exccause, HEX);
  Serial.println("================================");
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  show_info_startup();

  // Disable watchdog initially to prevent issues during initialization
  ESP.wdtDisable();

  // Initialize I2C with recovery if needed
  init_oled_device();

  // Re-enable watchdog after initialization
  ESP.wdtEnable(4000); // 4 second timeout
}

void loop()
{
  OLED_Clear(0);
  TEST_MainPage();

  OLED_Clear(0);
  Test_Color();

  OLED_Clear(0);
  Test_Rectangular();

  OLED_Clear(0);
  Test_Circle();

  OLED_Clear(0);
  Test_Triangle();

  OLED_Clear(0);
  TEST_English();

  OLED_Clear(0);
  TEST_Number_Character();

  OLED_Clear(0);
  TEST_BMP();

  OLED_Clear(0);
  TEST_Menu1();

  OLED_Clear(0);
  TEST_Menu2();
}