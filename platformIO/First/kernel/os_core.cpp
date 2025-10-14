#include "../inc/includes.h"

os_timer_t os_timer;

IRAM_ATTR void timerCallback(void *pArg)
{
}

void init_os_core(void)
{
  os_timer_setfn(&os_timer, timerCallback, NULL);
  os_timer_arm(&os_timer, 100, true);
}
