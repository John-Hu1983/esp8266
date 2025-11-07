#ifndef _DEV_TIMER_H_
#define _DEV_TIMER_H_

#define DEV_TIM_MAX 0xFFFFFFFF


void reset_timer_obj(osvar_t *tim);
bool is_timer_expired(osvar_t *tim, const osvar_t interval);
#endif
