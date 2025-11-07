#include "../inc/includes.h"

// Global variables
String configuredSSID = "";
String configuredPassword = "";
ESP8266WebServer wifiServer(WEB_SERVER_PORT);
DNSServer dnsServer;
WiFiNetwork foundNetworks[20];
int networkCount = 0;

/*
 * @brief Initialize EEPROM for storing WiFi credentials
 *
 * This function initializes the EEPROM memory to store the configured
 * WiFi SSID and password.
 */
void initEEPROM()
{
  EEPROM.begin(EEPROM_SIZE);
}

/*
 * @brief Save WiFi credentials to EEPROM
 *
 * This function saves the provided WiFi SSID and password to the EEPROM
 * memory.
 *
 * @param ssid The SSID of the WiFi network to be saved.
 * @param password The password of the WiFi network to be saved.
 */
void saveWiFiCredentials(const String &ssid, const String &password)
{
  initEEPROM();

  // Write SSID
  for (unsigned int i = 0; i < ssid.length() && i < 32; i++)
  {
    EEPROM.write(EEPROM_SSID_ADDR + i, ssid[i]);
  }
  EEPROM.write(EEPROM_SSID_ADDR + ssid.length(), 0); // Null terminator

  // Write password
  for (unsigned int i = 0; i < password.length() && i < 32; i++)
  {
    EEPROM.write(EEPROM_PASSWORD_ADDR + i, password[i]);
  }
  EEPROM.write(EEPROM_PASSWORD_ADDR + password.length(), 0); // Null terminator

  // Write magic number to indicate valid data
  EEPROM.write(EEPROM_MAGIC_ADDR, (MAGIC_NUMBER >> 8) & 0xFF);
  EEPROM.write(EEPROM_MAGIC_ADDR + 1, MAGIC_NUMBER & 0xFF);

  EEPROM.commit();
  Serial.println("WiFi credentials saved to EEPROM");
}

/*
 * @brief Load WiFi credentials from EEPROM
 *
 * This function loads the saved WiFi SSID and password from the EEPROM
 * memory.
 *
 * @param ssid Reference to a String variable where the loaded SSID will be stored.
 * @param password Reference to a String variable where the loaded password will be stored.
 * @return True if credentials were loaded successfully, false otherwise.
 */
bool loadWiFiCredentials(String &ssid, String &password)
{
  initEEPROM();

  // Check magic number
  uint16_t magic = (EEPROM.read(EEPROM_MAGIC_ADDR) << 8) | EEPROM.read(EEPROM_MAGIC_ADDR + 1);
  if (magic != MAGIC_NUMBER)
  {
    Serial.println("No saved WiFi credentials found");
    return false;
  }

  // Read SSID
  ssid = "";
  for (int i = 0; i < 32; i++)
  {
    char c = EEPROM.read(EEPROM_SSID_ADDR + i);
    if (c == 0)
      break;
    ssid += c;
  }

  // Read password
  password = "";
  for (int i = 0; i < 32; i++)
  {
    char c = EEPROM.read(EEPROM_PASSWORD_ADDR + i);
    if (c == 0)
      break;
    password += c;
  }

  Serial.println("WiFi credentials loaded from EEPROM");
  Serial.print("SSID: ");
  Serial.println(ssid);
  return true;
}

/*
 * @brief Reset WiFi credentials stored in EEPROM
 *
 * This function resets the stored WiFi SSID and password in the EEPROM
 * memory to their default values.
 */
void resetWiFiCredentials()
{
  initEEPROM();
  for (int i = 0; i < EEPROM_SIZE; i++)
  {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  Serial.println("WiFi credentials reset");
}

/*
 * @brief Blink status LED
 *
 * This function blinks the status LED (NodeMCU built-in LED) a specified
 * number of times with a given interval.
 *
 * @param times The number of times the LED should blink.
 * @param interval The interval (in milliseconds) between each blink.
 */
void blinkStatusLED(int times, int interval)
{
  if (STATUS_LED_PIN < 0)
    return;
  pinMode(STATUS_LED_PIN, OUTPUT);

  for (int i = 0; i < times; i++)
  {
    digitalWrite(STATUS_LED_PIN, LOW); // LED on (NodeMCU built-in LED is inverted)
    delay(interval);
    digitalWrite(STATUS_LED_PIN, HIGH); // LED off
    delay(interval);
  }
}

/*
 * @brief Get encryption type as string
 *
 * This function returns a string representation of the encryption type
 * based on the provided encryption type constant.
 *
 * @param encryption The encryption type constant to be converted.
 * @return The string representation of the encryption type.
 */
String getEncryptionType(uint8_t encryption)
{
  switch (encryption)
  {
  case ENC_TYPE_NONE:
    return "Open";
  case ENC_TYPE_WEP:
    return "WEP";
  case ENC_TYPE_TKIP:
    return "WPA/PSK";
  case ENC_TYPE_CCMP:
    return "WPA2/PSK";
  case ENC_TYPE_AUTO:
    return "WPA/WPA2";
  default:
    return "Unknown";
  }
}

/*
 * @brief Get signal bars based on RSSI value
 *
 * This function returns a string representation of signal bars based on the
 * provided RSSI value.
 *
 * @param rssi The RSSI value to be converted.
 * @return The string representation of signal bars.
 */
String getSignalBars(int32_t rssi)
{
  if (rssi > -50)
    return "📶📶📶📶";
  else if (rssi > -60)
    return "📶📶📶";
  else if (rssi > -70)
    return "📶📶";
  else
    return "📶";
}

/*
 * @brief Scan for available WiFi networks
 *
 * This function scans for available WiFi networks and stores the information
 * in the foundNetworks array.
 */
void scanAvailableNetworks()
{
  Serial.println("Scanning for WiFi networks...");

  // Temporarily switch to STA mode to scan
  WiFiMode_t previousMode = WiFi.getMode();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int n = WiFi.scanNetworks();
  networkCount = 0;

  if (n == 0)
  {
    Serial.println("No networks found");
  }
  else
  {
    Serial.print("Found ");
    Serial.print(n);
    Serial.println(" networks");

    // Store network information (limit to 20)
    networkCount = (n > 20) ? 20 : n;
    for (int i = 0; i < networkCount; i++)
    {
      foundNetworks[i].ssid = WiFi.SSID(i);
      foundNetworks[i].rssi = WiFi.RSSI(i);
      foundNetworks[i].encryption = WiFi.encryptionType(i);
      foundNetworks[i].channel = WiFi.channel(i);

      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(foundNetworks[i].ssid);
      Serial.print(" (RSSI: ");
      Serial.print(foundNetworks[i].rssi);
      Serial.println(" dBm)");
    }
  }

  WiFi.scanDelete();

  // Return to previous mode
  if (previousMode == WIFI_AP)
  {
    WiFi.mode(WIFI_AP);
  }
}

/*
 * @brief Get WiFi status as string
 *
 * This function returns a string representation of the WiFi status
 * based on the provided WiFi status constant.
 *
 * @param wifiStatus The WiFi status constant to be converted.
 * @return The string representation of the WiFi status.
 */
String getWiFiStatusString()
{
  switch (WiFi.status())
  {
  case WL_CONNECTED:
    return "Connected";
  case WL_NO_SHIELD:
    return "No Shield";
  case WL_IDLE_STATUS:
    return "Idle";
  case WL_NO_SSID_AVAIL:
    return "No SSID Available";
  case WL_SCAN_COMPLETED:
    return "Scan Completed";
  case WL_CONNECT_FAILED:
    return "Connection Failed";
  case WL_CONNECTION_LOST:
    return "Connection Lost";
  case WL_DISCONNECTED:
    return "Disconnected";
  default:
    return "Unknown Status";
  }
}

/*
 * @brief Test WiFi connection
 *
 * This function tests the WiFi connection by pinging Google DNS.
 *
 * @return True if the connection test is successful, false otherwise.
 */
bool testWiFiConnection()
{
  if (WiFi.status() != WL_CONNECTED)
    return false;

  // Test connection by pinging Google DNS
  IPAddress dns(8, 8, 8, 8);
  bool result = (WiFi.hostByName("google.com", dns) == 1);

  if (result)
  {
    Serial.println("WiFi connection test: PASSED");
  }
  else
  {
    Serial.println("WiFi connection test: FAILED");
  }
  return result;
}

/*
 * @brief Connect to WiFi station
 *
 * This function connects to a specified WiFi station using the provided SSID
 * and password.
 *
 * @param ssid The SSID of the WiFi station to connect to.
 * @param password The password of the WiFi station to connect to.
 * @return True if the connection is successful, false otherwise.
 */
bool connectToWiFiStation(const String &ssid, const String &password)
{
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - startTime > WIFI_CONNECT_TIMEOUT)
    {
      Serial.println("WiFi connection timeout");
      return false;
    }

    Serial.print(".");
    delay(500);
    yield();
  }

  Serial.println("\nWiFi connected successfully!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

/*
 * @brief Handle root page request
 *
 * This function handles the root page request by serving the HTML content
 * for the ESP8266 WiFi configuration page.
 */
void handleRootPage()
{
  Serial.println("Serving root page");

  String status = WiFi.status() == WL_CONNECTED ? "connected" : "disconnected";
  String statusText = WiFi.status() == WL_CONNECTED ? "✅ Connected to: " + WiFi.SSID() + "<br>IP: " + WiFi.localIP().toString() : "❌ Not connected";

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP8266 WiFi Config</title>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;background:#f0f0f0;margin:0;padding:20px}";
  html += ".container{max-width:400px;margin:0 auto;background:white;padding:20px;border-radius:10px;box-shadow:0 0 10px rgba(0,0,0,0.1)}";
  html += "h1{color:#333;text-align:center}";
  html += "label{display:block;margin:15px 0 5px;font-weight:bold}";
  html += "input{width:100%;padding:10px;border:1px solid #ddd;border-radius:5px;box-sizing:border-box}";
  html += "button{width:100%;padding:12px;background:#4CAF50;color:white;border:none;border-radius:5px;font-size:16px;cursor:pointer;margin-top:10px}";
  html += "button:hover{background:#45a049}";
  html += ".network-list{margin:20px 0;max-height:300px;overflow-y:auto}";
  html += ".network-item{padding:10px;margin:5px 0;background:#f9f9f9;border-radius:5px;cursor:pointer;border:1px solid #ddd}";
  html += ".network-item:hover{background:#e9e9e9}";
  html += ".signal-bars{display:inline-block;width:30px;text-align:right}";
  html += ".status{margin:20px 0;padding:10px;border-radius:5px;text-align:center}";
  html += ".status.connected{background:#d4edda;color:#155724;border:1px solid #c3e6cb}";
  html += ".status.disconnected{background:#f8d7da;color:#721c24;border:1px solid #f5c6cb}";
  html += ".scan-btn{background:#2196F3}";
  html += ".scan-btn:hover{background:#0b7dda}";
  html += ".loading{display:none;text-align:center;margin:20px}";
  html += ".spinner{border:4px solid #f3f3f3;border-top:4px solid #3498db;border-radius:50%;width:40px;height:40px;animation:spin 2s linear infinite;margin:0 auto}";
  html += "@keyframes spin{0%{transform:rotate(0deg)}100%{transform:rotate(360deg)}}";
  html += "</style>";
  html += "<script>";
  html += "function selectNetwork(ssid){document.getElementById('ssid').value=ssid}";
  html += "function validateForm(){";
  html += "var ssid=document.getElementById('ssid').value;";
  html += "var password=document.getElementById('password').value;";
  html += "if(ssid.length==0){alert('Please enter WiFi name (SSID)');return false}";
  html += "if(password.length<8&&password.length>0){alert('Password must be at least 8 characters');return false}";
  html += "return true}";
  html += "function scanNetworks(){";
  html += "document.getElementById('loading').style.display='block';";
  html += "var xhr=new XMLHttpRequest();";
  html += "xhr.open('GET','/scan',true);";
  html += "xhr.onreadystatechange=function(){";
  html += "if(xhr.readyState==4&&xhr.status==200){";
  html += "document.getElementById('loading').style.display='none';";
  html += "document.getElementById('networks').innerHTML=xhr.responseText}};";
  html += "xhr.send()}";
  html += "</script></head><body>";

  html += "<div class='container'>";
  html += "<h1>🔧 ESP8266 WiFi Setup</h1>";
  html += "<div class='status " + status + "'><strong>" + statusText + "</strong></div>";

  html += "<form action='/save' method='POST' onsubmit='return validateForm()'>";
  html += "<label>📶 WiFi Network Name (SSID):</label>";
  html += "<input type='text' id='ssid' name='ssid' value='" + configuredSSID + "' placeholder='Enter your WiFi name' required>";
  html += "<label>🔑 Password:</label>";
  html += "<input type='password' id='password' name='password' placeholder='Enter WiFi password (min 8 chars)'>";
  html += "<button type='submit'>💾 Connect to WiFi</button>";
  html += "</form>";

  html += "<button class='scan-btn' onclick='scanNetworks()'>🔄 Scan for Networks</button>";

  html += "<div class='loading' id='loading'>";
  html += "<div class='spinner'></div>";
  html += "<p>Scanning networks...</p>";
  html += "</div>";

  html += "<h3>📡 Available Networks:</h3>";
  html += "<div class='network-list' id='networks'>";

  // Add previously scanned networks
  if (networkCount > 0)
  {
    for (int i = 0; i < networkCount; i++)
    {
      html += "<div class='network-item' onclick='selectNetwork(\"" + foundNetworks[i].ssid + "\")'>";
      html += "<strong>" + foundNetworks[i].ssid + "</strong> ";
      html += "<span class='signal-bars'>" + getSignalBars(foundNetworks[i].rssi) + "</span><br>";
      html += "<small>" + getEncryptionType(foundNetworks[i].encryption) + " | Channel " + String(foundNetworks[i].channel) + " | " + String(foundNetworks[i].rssi) + " dBm</small>";
      html += "</div>";
    }
  }
  else
  {
    html += "<p>Click 'Scan for Networks' to find available WiFi networks</p>";
  }

  html += "</div>";
  html += "<hr><p style='text-align:center;font-size:12px;color:#666'>ESP8266 WiFi Manager v2.0</p>";
  html += "</div></body></html>";

  wifiServer.send(200, "text/html", html);
}

/*
 * @brief Handle network scan request
 *
 * This function handles the network scan request by scanning for available
 * WiFi networks and returning the results in HTML format.
 */
void handleScanNetworks()
{
  Serial.println("Handling network scan request");

  scanAvailableNetworks();

  String html = "";
  if (networkCount > 0)
  {
    for (int i = 0; i < networkCount; i++)
    {
      html += "<div class='network-item' onclick='selectNetwork(\"" + foundNetworks[i].ssid + "\")'>";
      html += "<strong>" + foundNetworks[i].ssid + "</strong> ";
      html += "<span class='signal-bars'>" + getSignalBars(foundNetworks[i].rssi) + "</span><br>";
      html += "<small>" + getEncryptionType(foundNetworks[i].encryption) + " | Channel " + String(foundNetworks[i].channel) + " | " + String(foundNetworks[i].rssi) + " dBm</small>";
      html += "</div>";
    }
  }
  else
  {
    html += "<p>No networks found. Try scanning again.</p>";
  }

  wifiServer.send(200, "text/html", html);
}

/*
 * @brief Handle save credentials request
 *
 * This function handles the save credentials request by saving the provided
 * SSID and password to the ESP8266's non-volatile storage.
 */
void handleSaveCredentials()
{
  Serial.println("Handling save credentials request");

  if (wifiServer.method() != HTTP_POST)
  {
    wifiServer.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  String ssid = wifiServer.arg("ssid");
  String password = wifiServer.arg("password");

  Serial.print("Received SSID: ");
  Serial.println(ssid);
  Serial.print("Password length: ");
  Serial.println(password.length());

  // Validate input
  if (ssid.length() == 0)
  {
    wifiServer.send(400, "text/html", "<html><body><h1>Error</h1><p>SSID cannot be empty</p><a href='/'>Go back</a></body></html>");
    return;
  }

  // Save credentials
  configuredSSID = ssid;
  configuredPassword = password;
  saveWiFiCredentials(ssid, password);

  // Show success page
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>WiFi Configuration Saved</title>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;background:#f0f0f0;margin:0;padding:20px}";
  html += ".container{max-width:400px;margin:0 auto;background:white;padding:20px;border-radius:10px;box-shadow:0 0 10px rgba(0,0,0,0.1);text-align:center}";
  html += "h1{color:#4CAF50}.success-icon{font-size:64px;color:#4CAF50;margin:20px}";
  html += "</style>";
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<div class='success-icon'>✅</div>";
  html += "<h1>WiFi Configuration Saved!</h1>";
  html += "<p>Connecting to: <strong>" + ssid + "</strong></p>";
  html += "<p>The device will restart and attempt to connect.</p>";
  html += "<p>Please wait...</p>";
  html += "</div></body></html>";

  wifiServer.send(200, "text/html", html);

  // Wait a bit then restart
  delay(3000);
  ESP.restart();
}

/*
 * @brief Handle 404 Not Found page request
 *
 * This function handles the 404 Not Found page request by returning an error
 * message with the requested URI, method, arguments, and argument values.
 */
void handleNotFoundPage()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += wifiServer.uri();
  message += "\nMethod: ";
  message += (wifiServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += wifiServer.args();
  message += "\n";

  for (uint8_t i = 0; i < wifiServer.args(); i++)
  {
    message += " " + wifiServer.argName(i) + ": " + wifiServer.arg(i) + "\n";
  }

  wifiServer.send(404, "text/plain", message);
}

/*
 * @brief Start web server
 *
 * This function starts the web server on port 80 and sets up the necessary
 * routes to handle root page, network scan, save credentials, and 404 errors.
 */
void startWebServer()
{
  wifiServer.on("/", handleRootPage);
  wifiServer.on("/scan", handleScanNetworks);
  wifiServer.on("/save", handleSaveCredentials);
  wifiServer.onNotFound(handleNotFoundPage);
  wifiServer.begin();
  Serial.println("Web server started on port 80");
}

/*
 * @brief Stop web server
 *
 * This function stops the web server and closes the connection to clients.
 */
void stopWebServer()
{
  wifiServer.stop();
  Serial.println("Web server stopped");
}

/*
 * @brief Start DNS server
 *
 * This function starts the DNS server on port 53 and sets up the necessary
 * settings to resolve requests for the configured domain.
 */
void startDNSServer()
{
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_SERVER_PORT, "*", AP_IP);
  Serial.println("DNS server started");
}

// Stop DNS server
/*
 * @brief Stop DNS server
 *
 * This function stops the DNS server and closes the connection to clients.
 */
void stopDNSServer()
{
  dnsServer.stop();
  Serial.println("DNS server stopped");
}

/*
 * @brief Start WiFi configuration portal
 *
 * This function starts the WiFi configuration portal by setting up the
 * Access Point with the configured SSID and password. It also starts the
 * DNS server and web server to handle client requests.
 */
void startWiFiConfigPortal()
{
  Serial.println("Starting WiFi configuration portal...");

  // Blink LED to indicate configuration mode
  blinkStatusLED(3, 200);

  // Set up Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_IP, AP_GATEWAY, AP_SUBNET);

  if (WiFi.softAP(AP_SSID, AP_PASSWORD))
  {
    Serial.println("Access Point started");
    Serial.print("AP SSID: ");
    Serial.println(AP_SSID);
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());

    // Start DNS and web servers
    startDNSServer();
    startWebServer();

    Serial.println("Configuration portal ready!");
    Serial.println("Connect to WiFi network: " + String(AP_SSID));
    Serial.println("Password: " + String(AP_PASSWORD));
    Serial.println("Then open browser and go to: http://192.168.4.1");

    // Blink LED to indicate portal is ready
    blinkStatusLED(5, 100);

    // Initial network scan
    scanAvailableNetworks();

    // Handle client requests
    unsigned long startTime = millis();
    while (millis() - startTime < CONFIG_PORTAL_TIMEOUT)
    {
      dnsServer.processNextRequest();
      wifiServer.handleClient();
      yield();

      // Blink LED slowly to indicate active portal
      if (millis() % 2000 < 100)
      {
        digitalWrite(STATUS_LED_PIN, LOW);
      }
      else
      {
        digitalWrite(STATUS_LED_PIN, HIGH);
      }
    }

    Serial.println("Configuration portal timeout");

    // Stop servers
    stopWebServer();
    stopDNSServer();

    // Blink LED to indicate timeout
    blinkStatusLED(10, 50);
  }
  else
  {
    Serial.println("Failed to start Access Point");
  }
}

/*
 * @brief Stop WiFi configuration portal
 *
 * This function stops the WiFi configuration portal by stopping the web server,
 * DNS server, and disconnecting the Access Point.
 */
void stopWiFiConfigPortal()
{
  Serial.println("Stopping WiFi configuration portal");
  stopWebServer();
  stopDNSServer();
  WiFi.softAPdisconnect(true);
}