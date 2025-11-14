#include "../inc/includes.h"
#include <ESP8266TimerInterrupt.h>

volatile uint32_t interruptCount = 0;
ESP8266Timer ITimer1;

const user_task_t TASK[] = {
    {0, os_idle_task},
    {5, process_main_menu_task}, // Changed from 2ms to 50ms to reduce CPU load
};
const u16 TASK_COUNT = sizeof(TASK) / sizeof(TASK[0]);
volatile osvar_t beacon[sizeof(TASK) / sizeof(TASK[0])];

/*
 * @brief: ISR function for hardware timer interrupt
 * @param: void
 * @return: void
 */
IRAM_ATTR void onHwTimer()
{
  interruptCount++;
}

/*
 * @brief: initialize OS core
 * @param: void
 * @return: void
 */
void init_os_core(void)
{
  if (!ITimer1.attachInterruptInterval(100, onHwTimer))
  {
    Serial.println("Failed to attach HW timer");
  }
}

/**
 * @brief: idle task function
 * @param: ms - time period in milliseconds
 * @return: void
 */
void os_idle_task(osvar_t ms)
{
  if (interruptCount >= 10)
  {
    cb_encoder_restricted_zoon();
    interruptCount = 0;
  }
}

/*
 * @brief: OS core task function
 * @param: void
 * @return: void
 */
void os_core_task(void)
{
  for (u16 i = 0; i < TASK_COUNT; i++)
  {
    ESP.wdtFeed();
    if (is_timer_expired((osvar_t *)&beacon[i], TASK[i].period))
    {
      TASK[i].entry(TASK[i].period);
      reset_timer_obj((osvar_t *)&beacon[i]);
    }
  }
}