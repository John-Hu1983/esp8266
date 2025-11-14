#include "../inc/includes.h"

const menu_iocon_t MAINMENU_GROUP[] PROGMEM = {
    {(u8 *)png_webside, (u8 *)"WEBSIDE"},
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
};

const btn_io KEY_HW = {GPIO_ENCODER_SW, false, 20, 50};
btn_obj_t EnterKey = {(btn_io *)&KEY_HW, 0, 0};

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
  draw_menu_choice();
}

/**
 * @brief: appear main menu
 * @param: void
 * @return: void
 */
void draw_menu_choice(void)
{
  const u8 *icon_ptr = (const u8 *)pgm_read_ptr(&MAINMENU_GROUP[menu_dev.cursor].png);
  const u8 *icon_name = (const u8 *)pgm_read_ptr(&MAINMENU_GROUP[menu_dev.cursor].title);
  OLED_Clear(0x0);
  Gui_Show_Str_At_Center(0, 0, WIDTH - 1, 16, (u8 *)icon_name, FONT_8X16, 1);
  gui_draw_btn_pic((WIDTH - 48) / 2, HEIGHT - 48, icon_ptr);
  OLED_update();
}

/**
 * @brief: leap out next menu
 * @param: void
 * @return: void
 */
void leapout_next_menu(void)
{
  menu_dev.cursor++;
  if (menu_dev.cursor >= sizeof(MAINMENU_GROUP) / sizeof(menu_iocon_t))
  {
    menu_dev.cursor = 0;
  }
}

/**
 * @brief: leap out previous menu
 * @param: void
 * @return: void
 */
void leapout_prev_menu(void)
{
  menu_dev.cursor = (menu_dev.cursor > 0)
                        ? menu_dev.cursor - 1
                        : sizeof(MAINMENU_GROUP) / sizeof(menu_iocon_t) - 1;
}

/**
 * @brief Choose kid menu
 * @param void
 * @retvalue void
 */
void _choose_kid_menu(encoder_t *encoder)
{
  if (encoder->state.bits.update)
  {
    encoder->state.bits.update = 0;

    if (encoder->state.bits.dir == ENC_TURN_CW)
    {
      leapout_next_menu();
      draw_menu_choice();
    }
    else if (encoder->state.bits.dir == ENC_TURN_CCW)
    {
      leapout_prev_menu();
      draw_menu_choice();
    }
  }
}

/**
 * @brief Process main menu task
 * @param ms Time interval in milliseconds
 * @retvalue None
 */
typedef enum
{
  MENU_STEP_IDLE = 0,
  MENU_STEP_WIFI = 1,
} Main_Menu_Step_t;
void process_main_menu_task(osvar_t ms)
{
  static u16 menu_step = MENU_STEP_IDLE;
  btnval_t btn_val = scan_button_condition(&EnterKey, ms);
  encoder_t *encoder = read_encoder_feature();
  int ret;

  switch (menu_step)
  {
  case MENU_STEP_IDLE:
    _choose_kid_menu(encoder);
    if (btn_val == BTN_CLICK)
    {
      menu_step = menu_dev.cursor + 1;
    }
    break;

  case MENU_STEP_WIFI:
    ret = app_wifi_loop(ms, btn_val, encoder);
    if (ret < 0)
    {
      menu_step = MENU_STEP_IDLE;
      draw_menu_choice();
    }
    break;

  default:
    break;
  }
}