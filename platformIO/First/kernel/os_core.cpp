#include "../inc/includes.h"

#define OUTPUT_PIN 2

volatile uint32_t interruptCount = 0;
ESP8266Timer ITimer1;

void os_test(dev_tim_t ms);

const user_task_t TASK[] = {
    {0, os_idle_task},
    {20, os_test},
};
const u16 TASK_COUNT = sizeof(TASK) / sizeof(TASK[0]);
volatile dev_tim_t beacon[sizeof(TASK) / sizeof(TASK[0])];

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
  pinMode(OUTPUT_PIN, OUTPUT);

  if (!ITimer1.attachInterruptInterval(488, onHwTimer))
  {
    Serial.println("Failed to attach HW timer");
  }
}

void os_test(dev_tim_t ms)
{
  digitalWrite(OUTPUT_PIN, !digitalRead(OUTPUT_PIN));
}

void os_idle_task(dev_tim_t ms)
{
}

/*
 * @brief: OS core task function
 * @param: void
 * @return: void
 */
void os_core_task(void)
{
  ESP.wdtFeed();

  for (u16 i = 0; i < TASK_COUNT; i++)
  {
    if (is_timer_expired((dev_tim_t *)&beacon[i], TASK[i].period))
    {
      TASK[i].entry(TASK[i].period);
      init_timer_object((dev_tim_t *)&beacon[i]);
    }
  }
}
