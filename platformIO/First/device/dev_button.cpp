#include "../inc/includes.h"
#define KEY_LONGCLI_MS (800)

/**
 * @brief Scan button condition
 * @param btn Button object
 * @param ms Time interval in milliseconds
 * @retval Button value
 */
btnval_t scan_button_condition(btn_obj_t *btn, osvar_t ms)
{
  btnval_t ret = BTN_IDLE;
  bool pressed = digitalRead(btn->io->pin) == btn->io->effective;
  btn->debounce += ms;

  switch (btn->step)
  {
  case 0:
    if (pressed)
    {
      btn->step = 1;
      btn->debounce = 0;
    }
    break;

  case 1:
    if (pressed)
    {
      if (btn->debounce >= KEY_LONGCLI_MS)
      {
        ret = BTN_HOLD;
        btn->step++;
        btn->debounce = 0;
      }
    }
    else
    {
      if (btn->debounce >= btn->io->threshold)
      {
        ret = BTN_CLICK;
        btn->step++;
        btn->debounce = 0;
      }
      else
      {
        btn->step = 0;
        btn->debounce = 0;
      }
    }
    break;

  default:
    if (pressed)
    {
      btn->debounce = 0;
    }
    else
    {
      if (btn->debounce >= btn->io->release)
      {
        ret = BTN_IDLE;
        btn->step = 0;
        btn->debounce = 0;
      }
    }
    break;
  }
  return ret;
}