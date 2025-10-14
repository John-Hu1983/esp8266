#include <Arduino.h>
#include <ESP8266WiFi.h>

// Your WiFi credentials
const char *ssid = "AI_PC2_7004955";
const char *password = "Hu13543710151";

// Connection timeout (milliseconds)
const unsigned long WIFI_CONNECT_TIMEOUT = 15000;

// Scan WiFi networks
void scanWiFiNetworks(void)
{
  Serial.println("Scanning WiFi networks...");

  // Set to Station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Start scanning
  int numNetworks = WiFi.scanNetworks();

  if (numNetworks == 0)
  {
    Serial.println("No WiFi networks found");
  }
  else
  {
    Serial.print("Found ");
    Serial.print(numNetworks);
    Serial.println(" WiFi networks");

    // Print scan results (sorted by signal strength)
    for (int i = 0; i < numNetworks; ++i)
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (Signal strength: ");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" dBm)");
      Serial.print(" Encryption: ");
      Serial.println(WiFi.encryptionType(i) == ENC_TYPE_NONE ? "None" : "Yes");
      delay(10);
    }
  }

  // Clear scan results to free memory
  WiFi.scanDelete();
}

// Connect to specified WiFi network
bool connectToWiFi(void)
{
  Serial.print("Connecting to WiFi network: ");
  Serial.println(ssid);

  // Set to Station mode and connect
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long startTime = millis();

  // Wait for connection success or timeout
  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - startTime > WIFI_CONNECT_TIMEOUT)
    {
      Serial.println("WiFi connection timeout");
      return false;
    }

    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWiFi connected successfully!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}
