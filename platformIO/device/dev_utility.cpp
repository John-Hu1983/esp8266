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

/*
 * @brief Show heap memory information
 *
 * This function displays the free heap memory and heap usage percentage.
 */
void show_heap_info(void)
{
  // Get free memory using ESP.getFreeHeap()
  Serial.print("Free Heap: ");
  Serial.println(ESP.getFreeHeap());

  // Alternative: use low-level SDK function (if ESP.getFreeHeap() is unavailable)
  Serial.print("Free Heap (SDK): ");
  Serial.println(system_get_free_heap_size());

  // For total heap memory, use an estimated value (ESP8266 typically has ~80KB available RAM)
  const unsigned int TOTAL_HEAP_ESTIMATE = 81920; // 80KB
  Serial.print("Heap Usage (%): ");
  Serial.println(100 - (ESP.getFreeHeap() * 100 / TOTAL_HEAP_ESTIMATE));
  Serial.println("");
}
