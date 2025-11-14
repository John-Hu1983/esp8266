
#ifndef __GUI_H__
#define __GUI_H__

void GUI_DrawPoint(u8 x, u8 y, u8 color);
void GUI_Fill(u8 sx, u8 sy, u8 ex, u8 ey, u8 color);
void GUI_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 color);
void GUI_DrawRectangle(u8 x1, u8 y1, u8 x2, u8 y2, u8 color);
void GUI_FillRectangle(u8 x1, u8 y1, u8 x2, u8 y2, u8 color);
void GUI_DrawCircle(u8 xc, u8 yc, u8 color, u8 r);
void GUI_FillCircle(u8 xc, u8 yc, u8 color, u8 r);
void GUI_DrawTriangel(u8 x0, u8 y0, u8 x1, u8 y1, u8 x2, u8 y2, u8 color);
void GUI_FillTriangel(u8 x0, u8 y0, u8 x1, u8 y1, u8 x2, u8 y2, u8 color);
void GUI_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size, u8 mode);
void GUI_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 Size, u8 mode);
void GUI_ShowString(u8 x, u8 y, u8 *chr, u8 Char_Size, u8 mode);
void Gui_Show_Str_At_Center(u8 xs, u8 ys, u8 xe, u8 ye, u8 *chr, u8 Char_Size, u8 mode);
void GUI_DrawBMP(u8 x, u8 y, u8 width, u8 height, u8 BMP[], u8 mode);
#endif
