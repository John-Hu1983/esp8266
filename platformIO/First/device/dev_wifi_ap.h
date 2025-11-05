#ifndef DEV_WIFI_AP_H
#define DEV_WIFI_AP_H

#include "../inc/includes.h"

// WiFi AP Configuration
#define AP_SSID "ESP8266_Config"
#define AP_PASSWORD "12345678"
#define AP_IP IPAddress(192, 168, 4, 1)
#define AP_GATEWAY IPAddress(192, 168, 4, 1)
#define AP_SUBNET IPAddress(255, 255, 255, 0)

// Web Server Configuration
#define WEB_SERVER_PORT 80
#define DNS_SERVER_PORT 53

// Configuration Portal Settings
#define CONFIG_PORTAL_TIMEOUT 300000  // 5 minutes
#define WIFI_CONNECT_TIMEOUT 30000      // 30 seconds

// EEPROM Configuration
#define EEPROM_SIZE 128
#define EEPROM_MAGIC_ADDR 0
#define EEPROM_SSID_ADDR 2
#define EEPROM_PASSWORD_ADDR 34
#define MAGIC_NUMBER 0x1234

// LED Pin (NodeMCU built-in LED)
#define STATUS_LED_PIN 2

// WiFi Network Structure
struct WiFiNetwork {
    String ssid;
    int32_t rssi;
    uint8_t encryption;
    uint8_t channel;
};

// External variables
extern ESP8266WebServer wifiServer;
extern DNSServer dnsServer;

// Function declarations
void startWiFiConfigPortal();
void stopWiFiConfigPortal();
void initEEPROM();
void saveWiFiCredentials(const String& ssid, const String& password);
bool loadWiFiCredentials(String& ssid, String& password);
void resetWiFiCredentials();
void blinkStatusLED(int times, int interval);
String getEncryptionType(uint8_t encryption);
String getSignalBars(int32_t rssi);
void scanAvailableNetworks();
String getWiFiStatusString();
bool testWiFiConnection();
bool connectToWiFiStation(const String& ssid, const String& password);
void handleRootPage();
void handleScanNetworks();
void handleSaveCredentials();
void handleNotFoundPage();
void startWebServer();
void stopWebServer();
void startDNSServer();
void stopDNSServer();

#endif