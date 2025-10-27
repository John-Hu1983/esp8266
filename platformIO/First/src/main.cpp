#include "../inc/includes.h"

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Detect restart reason
  rst_info *resetInfo = ESP.getResetInfoPtr();
  Serial.println("=== System Restart Detection ===");
  Serial.print("Restart reason: ");
  Serial.println(ESP.getResetReason());
  Serial.print("Exception cause: ");
  Serial.println(resetInfo->exccause, HEX);
  Serial.println("================================");

  // Disable watchdog initially to prevent issues during initialization
  ESP.wdtDisable();

  // Initialize I2C with recovery if needed
  init_oled_device();

  // Re-enable watchdog after initialization
  ESP.wdtEnable(4000); // 4 second timeout
}

void loop()
{
  // Feed watchdog at start of loop
  ESP.wdtFeed();

  // TEST_MainPage(); // 主界面显示测试
  // OLED_Clear(0);
  // ESP.wdtFeed(); // Feed during operations

  // Test_Color(); // 刷屏测试
  // OLED_Clear(0);
  // ESP.wdtFeed();

  // Continue feeding watchdog throughout all operations...
  Serial.println("rectangle test");
  Test_Rectangular(); // 矩形绘制测试
  OLED_Clear(0);
  ESP.wdtFeed();

  Serial.println("circle test");
  Test_Circle(); // 圆形绘制测试
  OLED_Clear(0);
  ESP.wdtFeed();

  Serial.println("triangle test");
  Test_Triangle(); // 三角形绘制测试
  OLED_Clear(0);
  ESP.wdtFeed();

  // TEST_English(); // 英文显示测试
  // OLED_Clear(0);
  // ESP.wdtFeed();

  // TEST_Number_Character(); // 数字和符号显示测试
  // OLED_Clear(0);
  // ESP.wdtFeed();

  // TEST_BMP(); // BMP单色图片显示测试
  // OLED_Clear(0);
  // ESP.wdtFeed();

  // TEST_Menu1(); // 菜单1显示测试
  // OLED_Clear(0);
  // ESP.wdtFeed();

  // TEST_Menu2(); // 菜单2显示测试
  // OLED_Clear(0);

  // delay(100);
}