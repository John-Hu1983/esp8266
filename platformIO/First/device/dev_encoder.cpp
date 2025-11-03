#include "../inc/includes.h"

#define GPIO_ENCODER_CLK 13
#define GPIO_ENCODER_DT 12
#define GPIO_ENCODER_SW 14

encoder_t encoder_dev;

/**
 * @brief Interrupt service routine for encoder
 * @retvalue None
 */
void IRAM_ATTR encoder_clk_ISR()
{
  encoder_dev.state.bits.update = 1;
  encoder_dev.state.bits.dir = (digitalRead(GPIO_ENCODER_DT) == LOW) ? ENC_TURN_CW : ENC_TURN_CCW;
}

/**
 * @brief Interrupt service routine for encoder DT pin
 * @retvalue None
 */
void IRAM_ATTR encoder_dt_ISR()
{
  encoder_dev.state.bits.update = 1;
  encoder_dev.state.bits.dir = (digitalRead(GPIO_ENCODER_CLK) == LOW) ? ENC_TURN_CW : ENC_TURN_CCW;
}

/**
 * @brief Initialize encoder object
 * @retvalue None
 */
void init_encode_object(void)
{
  pinMode(GPIO_ENCODER_CLK, INPUT);
  pinMode(GPIO_ENCODER_DT, INPUT);
  attachInterrupt(digitalPinToInterrupt(GPIO_ENCODER_CLK), encoder_clk_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(GPIO_ENCODER_DT), encoder_dt_ISR, RISING);
  memset((void *)&encoder_dev, 0, sizeof(encoder_dev));
}

/**
 * @brief Read encoder feature
 * @retvalue Pointer to encoder object
 */
encoder_t *read_encoder_feature(void)
{
  return &encoder_dev;
}
