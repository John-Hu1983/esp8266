#ifndef OS_CORE_H
#define OS_CORE_H

typedef struct
{
  u16 period;
  void (*entry)(void *arg);
} user_task_t;

void init_os_core(void);
#endif
