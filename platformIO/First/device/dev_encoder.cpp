#include "../inc/includes.h"

#define GPIO_ENA 2
#define GPIO_ENB 15

encoder_t encoder_dev;

void init_encode_object(void)
{
  pinMode(GPIO_ENA, INPUT_PULLUP);
  pinMode(GPIO_ENB, INPUT_PULLUP);
  memset((void *)&encoder_dev, 0, sizeof(encoder_dev));
}

void cb_scan_encode(void)
{
  u8 val = 0;

  if (digitalRead(GPIO_ENA) == LOW)
    val |= (u8)(1 << 0);
  if (digitalRead(GPIO_ENB) == LOW)
    val |= (u8)(1 << 1);

  if (val != encoder_dev.group[0])
  {
    for (u8 i = 3; i > 0; i--)
    {
      encoder_dev.group[i] = encoder_dev.group[i - 1];
    }
    encoder_dev.group[0] = val;
    encoder_dev.state.bits.update = 1;
  }
}

encoder_t *read_encoder_feature(void)
{
  return &encoder_dev;
}
