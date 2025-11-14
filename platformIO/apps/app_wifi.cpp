#include "../inc/includes.h"

enum
{
  _INIT_ = 0,
  _SELECT_,
};

static u8 step = 0;
static u8 choice = 0;
static Button *btnArray[3];

/*
 * @brief Create dynamically created buttons
 *
 * This function creates three dynamically created buttons with the labels "1", "2", and "3".
 *
 * @return int 0 if successful, -1 if buttons are not created
 */
int _create_dynamic_buttons(void)
{
  btnArray[0] = createDynamicButton(0, 0, WIDTH, 20, (u8 *)"Scan WIFI", 16, NULL,
                                    BUTTON_TYPE_TEXT, BUTTON_STATE_NORMAL, true);
  btnArray[1] = createDynamicButton(0, 20 * 1 + 1, WIDTH, 20, (u8 *)"AP Role", 16, NULL,
                                    BUTTON_TYPE_TEXT, BUTTON_STATE_NORMAL, true);
  btnArray[2] = createDynamicButton(0, 20 * 2 + 1, WIDTH, 20, (u8 *)"Property", 16, NULL,
                                    BUTTON_TYPE_TEXT, BUTTON_STATE_NORMAL, true);
  return (btnArray[0] && btnArray[1] && btnArray[2]);
}

/*
 * @brief Draw dynamically created buttons
 *
 * This function draws the dynamically created buttons on the OLED display.
 *
 * @param choice The index of the button to highlight (0-2)
 * @return int 0 if successful, -1 if buttons are not created
 */
int _draw_dynamic_buttons(u8 choice)
{
  if (!btnArray[0] || !btnArray[1] || !btnArray[2])
    return -1;

  OLED_Clear(0);
  for (int i = 0; i < 3; i++)
  {
    btnArray[i]->state = (i == choice) ? BUTTON_STATE_PRESSED : BUTTON_STATE_NORMAL;
    GUI_DrawButton(btnArray[i]);
  }
  OLED_update();
  return 0;
}

/*
 * @brief Release dynamically allocated buttons
 *
 * This function frees the memory allocated for the dynamically created buttons.
 */
void _release_dynamic_buttons(void)
{
  if (btnArray[0])
    freeButton(&btnArray[0]);
  if (btnArray[1])
    freeButton(&btnArray[1]);
  if (btnArray[2])
    freeButton(&btnArray[2]);
}

void _show_available_networks(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  if (n == 0)
  {
    // OLED_Clear(0);
    // GUI_ShowString(0, 0, (u8 *)"No networks found", 16, 1);
    // OLED_update();
    return;
  }

  // OLED_Clear(0);
  // for (int i = 0; i < n; i++)
  // {
  //   String ssid = WiFi.SSID(i);
  //   int32_t rssi = WiFi.RSSI(i);
  //   uint8_t encryption = WiFi.encryptionType(i);
  //   uint8_t channel = WiFi.channel(i);
  //   String encryptionType = getEncryptionType(encryption);
  //   String signalBars = getSignalBars(rssi);
  //   String info = ssid + " " + signalBars + " " + encryptionType + " " + String(channel);
  //   GUI_ShowString(0, i * 8, (u8 *)info.c_str(), 8, 1);
  // }
  // OLED_update();

  for (int i = 0; i < n; i++)
  {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (RSSI: ");
    Serial.print(WiFi.RSSI(i));
    Serial.println(" dBm)");
  }
}

/**
 * @brief Main loop for the WiFi application
 * @param ms Current time in milliseconds
 * @param btn Button value (BTN_CLICK, BTN_HOLD, etc.)
 * @param encoder Pointer to encoder_t struct (unused in this example)
 * @return int -1 if the application should quit, 0 otherwise
 */
int app_wifi_loop(osvar_t ms, btnval_t btn, encoder_t *encoder)
{
  int quit = 0;

  switch (step)
  {
  case _INIT_:
    if (_create_dynamic_buttons())
    {
      _draw_dynamic_buttons(0xff);
      step++;
    }
    else
    {
      _release_dynamic_buttons();
    }
    break;

  case _SELECT_:
    if (btn == BTN_HOLD)
    {
      step = 0xff;
    }

    if (btn == BTN_CLICK)
    {
      // show_heap_info();
      // if (choice == 0)
      {
        _show_available_networks();
      }
    }

    if (encoder->state.bits.update)
    {
      encoder->state.bits.update = 0;
      if (encoder->state.bits.dir == ENC_TURN_CW)
      {
        choice = (choice < 3) ? choice + 1 : 1;
      }
      else if (encoder->state.bits.dir == ENC_TURN_CCW)
      {
        choice = (choice > 1) ? choice - 1 : 3;
      }
      _draw_dynamic_buttons(choice - 1);
    }
    break;

  default:
    _release_dynamic_buttons();
    step = 0;
    choice = 0;
    quit = -1;
    break;
  }

  return quit;
}