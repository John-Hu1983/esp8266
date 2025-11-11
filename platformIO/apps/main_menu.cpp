#include "../inc/includes.h"
const u8 *const MainMenu_Icon[] PROGMEM = {
    png_camera,
    png_configuration,
    png_decimal,
    png_image,
    png_message,
    png_music,
    png_text,
    png_timer,
    png_topic,
    png_weather,
    png_webside,
};
u8 MainMenu_Icon_Index = 0;

const menu_iocon_t MAINMENU_GROUP[] PROGMEM = {
    {(u8 *)png_camera, (u8 *)"CAMERA"},
    {(u8 *)png_configuration, (u8 *)"CONFIG"},
    {(u8 *)png_decimal, (u8 *)"DECIMAL"},
    {(u8 *)png_image, (u8 *)"IMAGE"},
    {(u8 *)png_message, (u8 *)"MESSAGE"},
    {(u8 *)png_music, (u8 *)"MUSIC"},
    {(u8 *)png_text, (u8 *)"TEXT"},
    {(u8 *)png_timer, (u8 *)"TIMER"},
    {(u8 *)png_topic, (u8 *)"TOPIC"},
    {(u8 *)png_weather, (u8 *)"WEATHER"},
    {(u8 *)png_webside, (u8 *)"WEBSIDE"}};

const btn_io KEY_HW = {GPIO_ENCODER_SW, false, 20, 50};
btn_obj_t EnterKey = {(btn_io *)&KEY_HW, 0, 0};

const u8 *Btn_Text[8] = {(u8 *)"WIFI", (u8 *)"DBUG", (u8 *)"SCAN", (u8 *)"TEST", (u8 *)"SPIN", (u8 *)"MENU", (u8 *)"DEMO", (u8 *)"SETT"};
Button MainBtn[8];

MainMenu_Ctr menu_dev;

/*
 * @brief: initialize main menu
 * @param: void
 * @return: void
 */
void init_main_menu(void)
{
  pinMode(GPIO_ENCODER_SW, INPUT_PULLUP);
  memset((void *)&menu_dev, 0, sizeof(MainMenu_Ctr));
  draw_main_menu();
  OLED_update();
}

/**
 * @brief Triggle main menu slip
 * @param dir Slip direction
 * @retvalue None
 */
void _triggle_menu_slip(Slip_Dir_t dir)
{
  menu_dev.state.bits.update = 1;
  menu_dev.state.bits.dir = dir;
  menu_dev.step = 0;
  menu_dev.period = 0;
  switch (dir)
  {
  case Slip_Dir_Left:
    menu_dev.x = WIDTH - 1;
    menu_dev.y = HEIGHT - 48;
    break;

  case Slip_Dir_Right:
    menu_dev.x = 0;
    menu_dev.y = HEIGHT - 48;
    break;

  case Slip_Dir_Up:
    menu_dev.x = 0;
    menu_dev.y = HEIGHT - 1;
    break;

  case Slip_Dir_Down:
    menu_dev.x = 0;
    menu_dev.y = 0;
    break;

  default:
    break;
  }
}

/**
 * @brief Process main menu task
 * @param ms Time interval in milliseconds
 * @retvalue None
 */
void _handle_menu_slip(osvar_t ms)
{
  if (menu_dev.state.bits.update == 0)
    return;
}

/**
 * @brief Initialize main menu
 * @param None
 * @retvalue None
 */
void draw_main_menu(void)
{
  u8 row, col;
  OLED_Clear(0x0);
  for (row = 0; row < 2; row++)
  {
    for (col = 0; col < 4; col++)
    {
      MainBtn[row * 4 + col].x = Btn_Group_Xs + Btn_Width * col + Btn_Interval_X * col;
      MainBtn[row * 4 + col].y = Btn_Group_Ys + Btn_Height * row + Btn_Interval_Y * row;
      MainBtn[row * 4 + col].width = Btn_Width;
      MainBtn[row * 4 + col].height = Btn_Height;
      MainBtn[row * 4 + col].text = (u8 *)Btn_Text[row * 4 + col];
      MainBtn[row * 4 + col].textSize = 8;
      MainBtn[row * 4 + col].state = BUTTON_STATE_NORMAL;
      MainBtn[row * 4 + col].visible = true;
      GUI_DrawButton(&MainBtn[row * 4 + col]);
    }
  }
}

/**
 * @brief Process main menu task
 * @param ms Time interval in milliseconds
 * @retvalue None
 */
void process_main_menu_task(osvar_t ms)
{
  encoder_t *encoder;
  Button *btn;
  btnval_t btn_val;

  btn_val = scan_button_condition(&EnterKey, ms);
  if (btn_val == BTN_CLICK)
  {
    Serial.println("Encoder click!");
    // scanAvailableNetworks();
  }
  else if (btn_val == BTN_HOLD)
  {
    Serial.println("Encoder hold!");
  }

  encoder = read_encoder_feature();
  if (encoder->state.bits.update)
  {
    encoder->state.bits.update = 0;

    if (encoder->state.bits.dir == ENC_TURN_CW)
    {
      // Serial.println("Encoder turn CW!");
      // menu_dev.cursor++;
      // if (menu_dev.cursor > 8)
      // {
      //   menu_dev.cursor = 1;
      // }

      OLED_Clear(0x0);
      const u8 *icon_ptr = (const u8 *)pgm_read_ptr(&MAINMENU_GROUP[MainMenu_Icon_Index].png);
      const u8 *icon_name = (const u8 *)pgm_read_ptr(&MAINMENU_GROUP[MainMenu_Icon_Index].title);
      GUI_ShowString(30, 0, (u8 *)icon_name, 16, 1);
      gui_draw_btn_pic((WIDTH - 48) / 2, HEIGHT - 48, icon_ptr);
      OLED_update();
      MainMenu_Icon_Index++;
      if (MainMenu_Icon_Index >= sizeof(MainMenu_Icon) / sizeof(MainMenu_Icon[0]))
      {
        MainMenu_Icon_Index = 0;
      }
    }
    else if (encoder->state.bits.dir == ENC_TURN_CCW)
    {
      // Serial.println("Encoder turn CCW!");
      // menu_dev.cursor--;
      // if (menu_dev.cursor <= 0)
      // {
      //   menu_dev.cursor = 8;
      // }

      OLED_Clear(0x0);
      const u8 *icon_ptr = (const u8 *)pgm_read_ptr(&MAINMENU_GROUP[MainMenu_Icon_Index].png);
      const u8 *icon_name = (const u8 *)pgm_read_ptr(&MAINMENU_GROUP[MainMenu_Icon_Index].title);
      GUI_ShowString(30, 0, (u8 *)icon_name, 16, 1);
      gui_draw_btn_pic((WIDTH - 48) / 2, HEIGHT - 48, icon_ptr);
      OLED_update();
      if (MainMenu_Icon_Index > 0)
      {
        MainMenu_Icon_Index--;
      }
      else
      {
        MainMenu_Icon_Index = sizeof(MainMenu_Icon) / sizeof(MainMenu_Icon[0]) - 1;
      }
    }

    // btn = &MainBtn[menu_dev.cursor - 1];
    // draw_main_menu();
    // GUI_CheckButtonPress(btn, btn->x + btn->width / 2, btn->y + btn->height / 2);
    // GUI_DrawButton(btn);
    // OLED_update();
  }
}