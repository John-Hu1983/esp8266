#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_

#define Btn_Width 30
#define Btn_Height 30

#define Btn_Group_Xs 0
#define Btn_Group_Ys 0
#define Btn_Interval_X 1
#define Btn_Interval_Y 1

typedef struct
{
  u8 *png;
  u8 *title;
} menu_iocon_t;

typedef struct
{
  u8 cursor;
  union
  {
    u8 word;
    struct
    {
      unsigned update : 1; // 0~1
      unsigned dir : 3;    // 0~7
    } bits;
  } state;

  u8 step;
  u16 period;
  u16 x;
  u16 y;

} MainMenu_Ctr;

typedef enum
{
  Slip_Dir_Null = 0,
  Slip_Dir_Left,
  Slip_Dir_Right,
  Slip_Dir_Up,
  Slip_Dir_Down,
} Slip_Dir_t;

void init_main_menu(void);
void process_main_menu_task(osvar_t ms);
void draw_menu_choice(void);
void leapout_next_menu(void);
void leapout_prev_menu(void);

#endif
