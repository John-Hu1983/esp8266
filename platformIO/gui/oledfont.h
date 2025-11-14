#ifndef __OLEDFONT_H_
#define __OLEDFONT_H_

 
extern const u8 Font6x8[][8] PROGMEM;
extern const u8 Font8X16[] PROGMEM;

typedef enum
{
  FONT_6X8 = 8,
  FONT_8X16 = 16,
} Font_Type_t;

#endif
