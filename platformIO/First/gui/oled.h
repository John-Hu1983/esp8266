#ifndef _DEV_SH1106_H_
#define _DEV_SH1106_H_

#define SH1106_ID 0x3C

#define PAGE_SIZE 8
#define XLevelL 0x02
#define XLevelH 0x10
#define YLevel 0xB0
#define Brightness 0xFF
#define WIDTH 128
#define HEIGHT 64

#define OLED_CMD 0
#define OLED_DATA 1

void init_oled_device(void);
bool send_oled_command(u8 cmd);
bool send_oled_data(u8 data);
bool send_oled_command(u8 cmd);
bool send_oled_data(u8 data);
void OLED_WR_Byte(unsigned dat, unsigned cmd);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pixel(u8 x, u8 y, u8 color);
void OLED_Display(void);
void OLED_Clear(unsigned dat);
#endif
