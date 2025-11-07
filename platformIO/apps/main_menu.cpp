#include "../inc/includes.h"

const btn_io KEY_HW = {GPIO_ENCODER_SW, false, 20, 50};
btn_obj_t EnterKey = {(btn_io *)&KEY_HW, 0, 0};

const u8 *Btn_Text[8] = {(u8 *)"WIFI", (u8 *)"DBUG", (u8 *)"SCAN", (u8 *)"TEST", (u8 *)"SPIN", (u8 *)"MENU", (u8 *)"DEMO", (u8 *)"SETT"};
Button MainBtn[8];

MainMenu_Ctr MainMenu_dev;

/*
 * @brief: initialize main menu
 * @param: void
 * @return: void
 */
void init_main_menu(void)
{
  pinMode(GPIO_ENCODER_SW, INPUT_PULLUP);
  memset((void *)&MainMenu_dev, 0, sizeof(MainMenu_Ctr));
  draw_main_menu();
  OLED_update();
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
    scanAvailableNetworks();
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
      Serial.println("Encoder turn CW!");
      MainMenu_dev.cursor++;
      if (MainMenu_dev.cursor > 8)
      {
        MainMenu_dev.cursor = 1;
      }
    }
    else if (encoder->state.bits.dir == ENC_TURN_CCW)
    {
      Serial.println("Encoder turn CCW!");
      MainMenu_dev.cursor--;
      if (MainMenu_dev.cursor <= 0)
      {
        MainMenu_dev.cursor = 8;
      }
    }

    btn = &MainBtn[MainMenu_dev.cursor - 1];
    draw_main_menu();
    GUI_CheckButtonPress(btn, btn->x + btn->width / 2, btn->y + btn->height / 2);
    GUI_DrawButton(btn);
    OLED_update();
  }
}