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
  s8 cursor;
} MainMenu_Ctr;

void init_main_menu(void);
void draw_main_menu(void);
void process_main_menu_task(osvar_t ms);

#endif
