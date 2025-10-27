#ifndef _DEV_SH1106_H_
#define _DEV_SH1106_H_

#define SH1106_ID 0x3C
#define WIRE_MAX min(256, I2C_BUFFER_LENGTH)

#define PAGE_SIZE 8
#define SH1106_COLUMN_L 0x02
#define SH1106_COLUMN_H 0x10
#define SH1106_PAGE_ADDR 0xB0
#define Brightness 0xFF
#define WIDTH 128
#define HEIGHT 64

#define OLED_CMD 0
#define OLED_DATA 1

void init_oled_device(void);
bool send_oled_command(u8 cmd);
bool send_oled_data(u8 data);
void OLED_WR_Byte(unsigned dat, unsigned cmd);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pixel(u8 x, u8 y, u8 color);
void OLED_Clear(u8 dat);
void OLED_update(void);
#endif
