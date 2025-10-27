#include "../inc/includes.h"

static u8 OLED_buffer[WIDTH * HEIGHT / 8];

/**
 * @brief Initialize OLED device
 * @param None
 * @retvalue None
 */
void init_oled_device(void)
{
  memset(OLED_buffer, 0, sizeof(OLED_buffer));
  init_i2c_object(400000);
  delay(200);
  OLED_WR_Byte(0xAE, OLED_CMD); /*display off*/
  OLED_WR_Byte(0x02, OLED_CMD); /*set lower column address*/
  OLED_WR_Byte(0x10, OLED_CMD); /*set higher column address*/
  OLED_WR_Byte(0x40, OLED_CMD); /*set display start line*/
  OLED_WR_Byte(0xB0, OLED_CMD); /*set page address*/
  OLED_WR_Byte(0x81, OLED_CMD); /*contract control*/
  OLED_WR_Byte(0xFF, OLED_CMD); /*128*/
  OLED_WR_Byte(0xA1, OLED_CMD); /*set segment remap*/
  OLED_WR_Byte(0xA6, OLED_CMD); /*normal / reverse*/
  OLED_WR_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
  OLED_WR_Byte(0x3F, OLED_CMD); /*duty = 1/64*/
  OLED_WR_Byte(0xAD, OLED_CMD); /*set charge pump enable*/
  OLED_WR_Byte(0x8B, OLED_CMD); /*    0x8B    �ڹ�VCC   */
  OLED_WR_Byte(0x32, OLED_CMD); /*0X30---0X33  set VPP   8V */
  OLED_WR_Byte(0xC8, OLED_CMD); /*Com scan direction*/
  OLED_WR_Byte(0xD3, OLED_CMD); /*set display offset*/
  OLED_WR_Byte(0x00, OLED_CMD); /*   0x20  */
  OLED_WR_Byte(0xD5, OLED_CMD); /*set osc division*/
  OLED_WR_Byte(0x80, OLED_CMD);
  OLED_WR_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
  OLED_WR_Byte(0x1F, OLED_CMD); /*0x22*/
  OLED_WR_Byte(0xDA, OLED_CMD); /*set COM pins*/
  OLED_WR_Byte(0x12, OLED_CMD);
  OLED_WR_Byte(0xDB, OLED_CMD); /*set vcomh*/
  OLED_WR_Byte(0x40, OLED_CMD);
  OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/

  OLED_Clear(0);
}

/**
 * @brief Send OLED command
 * @param cmd Command byte to send
 * @return True if command is sent successfully, false otherwise
 */
bool send_oled_command(u8 cmd)
{
  byte error = 0;
  Wire.beginTransmission(SH1106_ID);
  Wire.write(0x00);
  Wire.write(cmd);
  error = Wire.endTransmission();
  return error == 0;
}

/**
 * @brief Send OLED data
 * @param data Data byte to send
 * @return True if data is sent successfully, false otherwise
 */
bool send_oled_data(u8 data)
{
  byte error = 0;
  Wire.beginTransmission(SH1106_ID);
  Wire.write(0x40);
  Wire.write(data);
  error = Wire.endTransmission();
  return error == 0;
}

/*
 * @brief Send OLED data burst
 * @param data Pointer to data buffer
 * @param len Length of data to send
 * @return True if data burst is sent successfully, false otherwise
 */
bool send_oled_dat_burst(const u8 *data, u16 len)
{
  byte error = 0;
  Wire.beginTransmission(SH1106_ID);
  Wire.write(0x40);
  while (len--)
  {
    Wire.write(*data++);
  }
  error = Wire.endTransmission();
  return error == 0;
}

/**
 * @brief Write a byte of content to the OLED screen
 * @param dat Content to be written
 * @param cmd 0-write command
 *            1-write data
 * @retvalue None
 */
void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
  if (cmd)
  {
    send_oled_data(dat);
  }
  else
  {
    send_oled_command(dat);
  }
}

/**
 * @brief Set coordinates in the OLED screen
 * @param x x coordinates
 * @param y y coordinates
 * @retvalue None
 */
void OLED_Set_Pos(u8 x, u8 y)
{
  OLED_WR_Byte(SH1106_PAGE_ADDR + y / PAGE_SIZE, OLED_CMD);
  OLED_WR_Byte((((x + 2) & 0xf0) >> 4) | 0x10, OLED_CMD);
  OLED_WR_Byte(((x + 2) & 0x0f), OLED_CMD);
}

/**
 * @brief Turn on OLED display
 * @param None
 * @retvalue None
 */
void OLED_Display_On(void)
{
  OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC����
  OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
  OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}

/**
 * @brief Turn off OLED display
 * @param None
 * @retvalue None
 */
void OLED_Display_Off(void)
{
  OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC����
  OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
  OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}

/**
 * @brief Set the value of pixel to RAM
 * @param x the x coordinates of pixel
 * @param y the y coordinates of pixel
 * @param color the color value of the point
 *              1-white
 *              0-black
 * @retvalue None
 */
void OLED_Set_Pixel(u8 x, u8 y, u8 color)
{
  if (color)
  {
    OLED_buffer[(y / PAGE_SIZE) * WIDTH + x] |= (1 << (y % PAGE_SIZE)) & 0xff;
  }
  else
  {
    OLED_buffer[(y / PAGE_SIZE) * WIDTH + x] &= ~((1 << (y % PAGE_SIZE)) & 0xff);
  }
}

/**
 * @brief Display in OLED screen
 * @param None
 * @retvalue None
 */
void OLED_update(void)
{
  u8 i;
  for (i = 0; i < PAGE_SIZE; i++)
  {
    OLED_WR_Byte(SH1106_PAGE_ADDR + i, OLED_CMD);
    OLED_WR_Byte(SH1106_COLUMN_L, OLED_CMD);
    OLED_WR_Byte(SH1106_COLUMN_H, OLED_CMD);
    send_oled_dat_burst((u8 *)&OLED_buffer[i * WIDTH], WIDTH);
  }
}

/**
 * @brief Clear OLED screen
 * @param dat 0-Display full black
 *            1-Display full white
 * @retvalue None
 */
void OLED_Clear(u8 dat)
{
  if (dat)
  {
    memset(OLED_buffer, 0xff, sizeof(OLED_buffer));
  }
  else
  {
    memset(OLED_buffer, 0, sizeof(OLED_buffer));
  }
}
