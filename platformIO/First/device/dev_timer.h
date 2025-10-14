#ifndef _DEV_TIMER_H_
#define _DEV_TIMER_H_

typedef unsigned long dev_tim_t;
#define DEV_TIM_MAX 0xFFFFFFFF


void init_timer_object(dev_tim_t *tim);
bool is_timer_expired(dev_tim_t *tim, const dev_tim_t interval);
#endif
