#ifndef _DEV_ENCODER_H_
#define _DEV_ENCODER_H_

typedef struct
{
  union
  {
    u8 word;
    struct
    {
      unsigned update : 1;
      unsigned dir : 1;
      unsigned step : 2;
    } bits;
  } state;

  u8 previous_val;
  u8 group[4];

} encoder_t;

void init_encode_object(void);
void cb_scan_encode(void);
encoder_t *read_encoder_feature(void);
#endif
