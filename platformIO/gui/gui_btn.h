#ifndef _GUI_BTN_H_
#define _GUI_BTN_H_

// Set global GUI background color
extern u8 g_GUIBackgroundColor;
void GUI_SetBackgroundColor(u8 color);

// Button state enumeration
typedef enum
{
  BUTTON_STATE_NORMAL = 0,
  BUTTON_STATE_PRESSED = 1,
  BUTTON_STATE_DISABLED = 2
} ButtonState;

// Button type enumeration
typedef enum
{
  BUTTON_TYPE_TEXT = 0,
  BUTTON_TYPE_IMAGE = 1
} ButtonType;

// Button structure
typedef struct
{
  u8 x;              // Button top-left x coordinate
  u8 y;              // Button top-left y coordinate
  u8 width;          // Button width
  u8 height;         // Button height
  u8 *text;          // Button text (for text buttons)
  u8 textSize;       // Text size (8 or 16)
  u8 *imageData;     // Image data pointer (for image buttons)
  ButtonType type;   // Button type (text or image)
  ButtonState state; // Button state
  bool visible;      // Whether visible
} Button;

void GUI_DrawButton(Button *btn);
bool GUI_CheckButtonPress(Button *btn, u8 x, u8 y);
void GUI_ResetButtonState(Button *btn);
void TEST_ButtonDemo(void);
void TEST_ImageButtonDemo(void); // New demo for image buttons
void gui_draw_btn_pic(u8 x, u8 y, const u8 *bmp);

#endif