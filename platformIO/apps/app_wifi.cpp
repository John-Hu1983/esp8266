#include "../inc/includes.h"

static u8 step = 0;
static u8 choice = 0;
static Button *btnArray[3];
static event_op_t op = {0};
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

/*
 * @brief Process the WiFi application loop
 *
 * This function processes the WiFi application loop, handling button presses and encoder input.
 *
 * @param ms Current time in milliseconds
 * @param btn Button value (BTN_CLICK, BTN_HOLD, etc.)
 * @param encoder Pointer to encoder_t struct
 * @return Wifi_Step_t The next step in the WiFi application
 */
Wifi_Step_t _process_wifi_app_loop(osvar_t ms, btnval_t btn, encoder_t *encoder)
{
  Wifi_Step_t next_step = WIFI_STEP_LOOP;

  if (btn == BTN_HOLD)
  {
    next_step = WIFI_STEP_QUIT;
  }
  else if (btn == BTN_CLICK)
  {
    memset(&op, 0, sizeof(event_op_t));
    switch (choice)
    {
    case WIFI_AET_SCAN:
      next_step = WIFI_STEP_NETWORKS;
      break;
    case WIFI_AET_AP_ROLE:
      next_step = WIFI_STEP_AP_ROLE;
      break;
    case WIFI_AET_PROPERTY:
      next_step = WIFI_STEP_PROPERTY;
      break;
    default:
      break;
    }
  }

  if (encoder->state.bits.update)
  {
    encoder->state.bits.update = 0;
    if (encoder->state.bits.dir == ENC_TURN_CW)
    {
      choice = (choice < WIFI_AET_PROPERTY) ? choice + 1 : WIFI_AET_SCAN;
    }
    else if (encoder->state.bits.dir == ENC_TURN_CCW)
    {
      choice = (choice > WIFI_AET_SCAN) ? choice - 1 : WIFI_AET_PROPERTY;
    }
    _draw_dynamic_buttons(choice - 1);
  }
  return next_step;
}

/**
 * @brief Show available networks
 * @param ms Current time in milliseconds
 * @param btn Button value (BTN_CLICK, BTN_HOLD, etc.)
 * @param encoder Pointer to encoder_t struct (unused in this example)
 * @return int -1 if no networks are available, 0 otherwise
 */
int _show_available_networks(osvar_t ms, btnval_t btn, encoder_t *encoder)
{
#define WIFI_LIST_MAX 4
  int res = -1;
  static u8 wifi_list_index = 0;
  static u8 *networks = NULL;
  static u8 **ssid_list = NULL;
  static TextEditWidget *textedit[WIFI_LIST_MAX] = {NULL};

  op.debounce = (op.debounce < ms) ? 0 : (op.debounce - ms);
  switch (op.step)
  {
  case 0:
    OLED_Clear(0);
    GUI_ShowString(0, 0, (u8 *)"SCANNING WIFI", FONT_8X16, 1);
    GUI_ShowString(0, 20, (u8 *)"PLEASE WAIT ...", FONT_8X16, 1);
    OLED_update();
    op.step++;
    break;

  case 1:
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    op.debounce = 100;
    if (networks == NULL)
    {
      networks = (u8 *)malloc(sizeof(u8));
    }

    if (networks)
    {
      op.step++;
    }
    break;

  case 2:
    if (op.debounce == 0)
    {
      *networks = WiFi.scanNetworks();
      GUI_ShowString(0, 40, (u8 *)"WIFI: ", FONT_8X16, 1);
      GUI_ShowNum(48, 40, *networks, 3, FONT_8X16, 1);
      OLED_update();
      op.debounce = 2000;
      op.step++;
    }
    break;

  case 3:
    if (op.debounce == 0)
    {
      ssid_list = (u8 **)malloc(*networks * sizeof(u8 *));
      for (int i = 0; i < *networks; i++)
      {
        ssid_list[i] = (u8 *)malloc(WiFi.SSID(i).length() + 1);
        strcpy((char *)ssid_list[i], (char *)WiFi.SSID(i).c_str());
      }

      wifi_list_index = 0;
      u8 height_per_item = HEIGHT / WIFI_LIST_MAX;
      for (int i = 0; i < WIFI_LIST_MAX; i++)
      {
        if (textedit[i] == NULL)
        {
          textedit[i] = createTextEdit(0, height_per_item * i, WIDTH - 1, height_per_item, 21);
        }
      }

      OLED_Clear(0);
      for (int i = 0; i < WIFI_LIST_MAX; i++)
      {
        u8 inx = wifi_list_index + i;
        if (inx >= *networks)
        {
          break;
        }
        strcpy((char *)textedit[i]->buffer, (const char *)ssid_list[inx]);
        drawTextEdit(textedit[i]);
      }
      OLED_update();

      op.debounce = 200;
      op.step++;
    }
    break;

  case 4:
    if (btn == BTN_HOLD)
    {
      op.step = 0xff;
    }

    if (encoder->state.bits.update)
    {
      encoder->state.bits.update = 0;
      if (encoder->state.bits.dir == ENC_TURN_CW)
      {
        wifi_list_index = (wifi_list_index < *networks - WIFI_LIST_MAX) ? wifi_list_index + 1 : 0;
      }
      else if (encoder->state.bits.dir == ENC_TURN_CCW)
      {
        wifi_list_index = (wifi_list_index > 0) ? wifi_list_index - 1 : *networks - WIFI_LIST_MAX;
      }

      OLED_Clear(0);
      for (int i = 0; i < WIFI_LIST_MAX; i++)
      {
        u8 inx = wifi_list_index + i;
        if (inx >= *networks)
        {
          break;
        }
        strcpy((char *)textedit[i]->buffer, (const char *)ssid_list[inx]);
        drawTextEdit(textedit[i]);
      }
      OLED_update();
    }

    break;

  default:
    if (op.debounce == 0)
    {
      u8 network_count = *networks;
      for (int i = 0; i < network_count; i++)
      {
        if (ssid_list[i] != NULL)
        {
          free(ssid_list[i]);
          ssid_list[i] = NULL;
        }
      }

      for (int i = 0; i < WIFI_LIST_MAX; i++)
      {
        if (textedit[i] != NULL)
        {
          freeTextEdit(textedit[i]);
          textedit[i] = NULL;
        }
      }

      free(ssid_list);
      ssid_list = NULL;

      free(networks);
      networks = NULL;
      res = 0;
    }
    break;
  }

  // int n = WiFi.scanNetworks();
  // if (n == 0)
  // {
  //   return;
  // }

  // for (int i = 0; i < n; i++)
  // {
  //   Serial.print(i + 1);
  //   Serial.print(": ");
  //   Serial.print(WiFi.SSID(i));
  //   Serial.print(" (RSSI: ");
  //   Serial.print(WiFi.RSSI(i));
  //   Serial.println(" dBm)");
  // }

  // TextEditWidget *myTextEdit = NULL;
  // myTextEdit = createTextEdit(0, 0, WIDTH - 1, 10, 8);
  // strcpy((char *)myTextEdit->buffer, "Hello World!");
  // OLED_Clear(0);
  // drawTextEdit(myTextEdit);
  // OLED_update();

  return res;
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
  case WIFI_STEP_INIT:
    if (_create_dynamic_buttons())
    {
      _draw_dynamic_buttons(0xff);
      step = WIFI_STEP_LOOP;
    }
    else
    {
      _release_dynamic_buttons();
    }
    break;

  case WIFI_STEP_LOOP:
    step = _process_wifi_app_loop(ms, btn, encoder);
    break;

  case WIFI_STEP_NETWORKS:
    if (_show_available_networks(ms, btn, encoder) == 0)
    {
      choice = 0;
      _draw_dynamic_buttons(0xff);
      show_heap_info();
      step = WIFI_STEP_LOOP;
    }
    break;

  case WIFI_STEP_AP_ROLE:
    break;

  case WIFI_STEP_PROPERTY:
    break;

  default:
  case WIFI_STEP_QUIT:
    _release_dynamic_buttons();
    step = 0;
    choice = 0;
    quit = -1;
    break;
  }

  return quit;
}