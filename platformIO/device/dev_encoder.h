#ifndef _DEV_ENCODER_H_
#define _DEV_ENCODER_H_

#define GPIO_ENCODER_CLK 13
#define GPIO_ENCODER_DT 12
#define GPIO_ENCODER_SW 0

typedef enum
{
  ENC_TURN_NONE = 0,
  ENC_TURN_CW,
  ENC_TURN_CCW,
} ENC_Dir_t;

typedef struct
{
  union
  {
    u8 word;
    struct
    {
      unsigned update : 1;
      unsigned dir : 2;
      unsigned step : 2;
    } bits;
  } state;

  u8 restricted;
} encoder_t;

void init_encode_object(void);
void cb_encoder_restricted_zoon(void);
encoder_t *read_encoder_feature(void);
#endif
