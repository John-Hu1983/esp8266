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