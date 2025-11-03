#ifndef _DEV_ENCODER_H_
#define _DEV_ENCODER_H_

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

  u8 previous_val;
  u8 group[4];

} encoder_t;

void init_encode_object(void);
encoder_t *read_encoder_feature(void);
#endif
