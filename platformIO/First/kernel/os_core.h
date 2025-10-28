#ifndef OS_CORE_H
#define OS_CORE_H

typedef struct
{
  u16 period;
  void (*entry)(dev_tim_t);
} user_task_t;

void init_os_core(void);
void os_idle_task(dev_tim_t ms);
void os_core_task(void);
#endif
