#ifndef _DEV_BUTTON_H_
#define _DEV_BUTTON_H_

typedef enum
{
  BTN_IDLE,
  BTN_CLICK,
  BTN_HOLD,
  BTN_END
} btnval_t;

typedef struct
{
  u8 pin;
  bool effective;
  u16 threshold;
  u16 release;
} btn_io;

typedef struct
{
  btn_io *io;
  u8 step;
  u16 debounce;
} btn_obj_t;

btnval_t scan_button_condition(btn_obj_t *btn, osvar_t ms);
#endif
