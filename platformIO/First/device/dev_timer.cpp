#include "../inc/includes.h"

/*
 * @brief: initialize
 * @param: void
 * @return: void
 */
void init_timer_object(dev_tim_t *tim)
{
  dev_tim_t currentMillis = millis();
  *tim = currentMillis;
}

/*
 * @brief:
 * @param:
 * @return:
 */
bool is_timer_expired(dev_tim_t *tim, const dev_tim_t interval)
{
  bool expired = false;
  dev_tim_t gap;

  dev_tim_t currentMillis = millis();
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
