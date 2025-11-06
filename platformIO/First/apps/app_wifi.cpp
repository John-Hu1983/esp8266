#include "../inc/includes.h"

void init_wifi_device(void)
{
  // Try to load saved WiFi credentials
  String savedSSID, savedPassword;
  if (loadWiFiCredentials(savedSSID, savedPassword))
  {
    Serial.println("Found saved WiFi credentials");
    Serial.print("SSID: ");
    Serial.println(savedSSID);

    // Try to connect to saved WiFi
    if (connectToWiFiStation(savedSSID, savedPassword))
    {
      Serial.println("Successfully connected to saved WiFi!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      // Your main application code here
      // The device is now connected to WiFi
      return;
    }
    else
    {
      Serial.println("Failed to connect to saved WiFi");
    }
  }
  else
  {
    Serial.println("No saved WiFi credentials found");
  }

  // If no saved credentials or connection failed, start configuration portal
  Serial.println("Starting WiFi configuration portal...");
  startWiFiConfigPortal();

  // After configuration portal, try to connect again
  if (loadWiFiCredentials(savedSSID, savedPassword))
  {
    if (connectToWiFiStation(savedSSID, savedPassword))
    {
      Serial.println("Successfully connected to configured WiFi!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    }
    else
    {
      Serial.println("Failed to connect even after configuration");
      // Handle error - maybe restart or go into deep sleep
    }
  }
}

void service_wifi_task(osvar_t ms)
{
  // Your main application loop
  // The device should be connected to WiFi at this point

  // Example: Print WiFi status every 10 seconds
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 10000)
  {
    lastPrint = millis();
    Serial.print("WiFi Status: ");
    Serial.println(getWiFiStatusString());

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.print("Connected to: ");
      Serial.println(WiFi.SSID());
      Serial.print("Signal strength: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
    }
  }

  // Handle web server if in AP mode (for reconfiguration)
  if (WiFi.getMode() == WIFI_AP)
  {
    wifiServer.handleClient();
  }
}
