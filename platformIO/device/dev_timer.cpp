#include "../inc/includes.h"

/*
 * @brief: initialize
 * @param: void
 * @return: void
 */
void reset_timer_obj(osvar_t *tim)
{
  osvar_t currentMillis = millis();
  *tim = currentMillis;
}

/*
 * @brief:
 * @param:
 * @return:
 */
bool is_timer_expired(osvar_t *tim, const osvar_t interval)
{
  bool expired = false;
  osvar_t gap;

  osvar_t currentMillis = millis();
  if (*tim <= currentMillis)
  {
    if (currentMillis - *tim >= interval)
    {
      expired = true;
      *tim = currentMillis;
    }
  }
  else
  {
    gap = DEV_TIM_MAX - *tim + currentMillis;
    if (gap >= interval)
    {
      expired = true;
      *tim = currentMillis;
    }
  }
  return expired;
}
