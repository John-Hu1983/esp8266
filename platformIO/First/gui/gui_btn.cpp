#include "../inc/includes.h"

// Global GUI background color variable
u8 g_GUIBackgroundColor = 0; // Default black background

/**
 * @name       :void GUI_SetBackgroundColor(u8 color)
 * @function   :Set global GUI background color
 * @parameters :color:Background color (0-black, 1-white)
 * @retvalue   :None
 */
void GUI_SetBackgroundColor(u8 color)
{
  g_GUIBackgroundColor = (color == 0) ? 0 : 1; // Ensure only 0 or 1 values
}

// Modify background color variable used in GUI_DrawButton function
void GUI_DrawButton(Button *btn)
{
  if (!btn || !btn->visible)
    return;

  // Use global GUI background color
  u8 guiBackgroundColor = g_GUIBackgroundColor;
  u8 guiForegroundColor = !guiBackgroundColor;

  // Set colors based on button state
  u8 buttonBackgroundColor, buttonTextColor, buttonBorderColor;

  if (btn->state == BUTTON_STATE_PRESSED)
  {
    // Pressed state: Button background color becomes GUI background color, text color becomes GUI foreground color
    buttonBackgroundColor = guiBackgroundColor;
    buttonTextColor = guiForegroundColor;
    buttonBorderColor = guiForegroundColor;
  }
  else if (btn->state == BUTTON_STATE_DISABLED)
  {
    // Disabled state: Light gray background (implemented with dots), gray text
    u8 i, j;
    for (i = btn->y; i < btn->y + btn->height; i++)
    {
      for (j = btn->x; j < btn->x + btn->width; j++)
      {
        if ((i + j) % 2 == 0)
        {
          OLED_Set_Pixel(j, i, guiForegroundColor);
        }
        else
        {
          OLED_Set_Pixel(j, i, guiBackgroundColor);
        }
      }
    }
    GUI_DrawRectangle(btn->x, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, guiBackgroundColor);

    // Calculate text center position
    u8 textWidth = 0;
    u8 textHeight = btn->textSize;
    u8 *text = btn->text;
    while (*text)
    {
      textWidth += (btn->textSize == 16) ? 8 : 6;
      text++;
    }

    u8 textX = btn->x + (btn->width - textWidth) / 2;
    u8 textY = btn->y + (btn->height - textHeight) / 2;

    // Disabled state: Gray text (implemented with alternating pixels)
    text = btn->text;
    u8 charIndex = 0;
    while (*text)
    {
      u8 currentChar = *text;
      for (u8 i = 0; i < textHeight; i++)
      {
        u8 rowData;
        if (textHeight == 16)
        {
          rowData = F8X16[(currentChar - ' ') * 16 + i];
        }
        else
        {
          rowData = F6x8[currentChar - ' '][i];
        }

        for (u8 j = 0; j < 8; j++)
        {
          if (rowData & (0x80 >> j))
          {
            u8 dotX = textX + charIndex * ((textHeight == 16) ? 8 : 6) + j;
            u8 dotY = textY + i;
            if (dotX < btn->x + btn->width && dotY < btn->y + btn->height)
            {
              // Use alternating pixels to create gray effect
              if ((dotX + dotY) % 4 == 0)
              {
                OLED_Set_Pixel(dotX, dotY, guiBackgroundColor);
              }
              else
              {
                OLED_Set_Pixel(dotX, dotY, guiForegroundColor);
              }
            }
          }
        }
      }
      charIndex++;
      text++;
    }
    return; // Already processed disabled state text, return directly
  }
  else
  {
    // Normal state: Button background color is opposite to GUI background color, text color is the same as GUI background color
    buttonBackgroundColor = guiForegroundColor;
    buttonTextColor = guiBackgroundColor;
    buttonBorderColor = guiBackgroundColor;
  }

  // Draw button background
  GUI_FillRectangle(btn->x, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, buttonBackgroundColor);

  // Draw button border
  GUI_DrawRectangle(btn->x, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, buttonBorderColor);

  // Calculate text center position
  u8 textWidth = 0;
  u8 textHeight = btn->textSize;
  u8 *text = btn->text;
  while (*text)
  {
    textWidth += (btn->textSize == 16) ? 8 : 6; // Calculate total text width
    text++;
  }

  u8 textX = btn->x + (btn->width - textWidth) / 2;
  u8 textY = btn->y + (btn->height - textHeight) / 2;

  // Draw button text - Text color is the same as GUI background color
  GUI_ShowString(textX, textY, btn->text, btn->textSize, buttonTextColor);
}

/**
 * @name       :bool GUI_CheckButtonPress(Button *btn, u8 x, u8 y)
 * @function   :Check if button is pressed
 * @parameters :btn:Button pointer
 *              x:Click x coordinate
 *              y:Click y coordinate
 * @retvalue   :Whether button is clicked
 */
bool GUI_CheckButtonPress(Button *btn, u8 x, u8 y)
{
  if (!btn || !btn->visible || btn->state == BUTTON_STATE_DISABLED)
    return false;

  // Check if coordinates are within button range
  if (x >= btn->x && x <= btn->x + btn->width - 1 &&
      y >= btn->y && y <= btn->y + btn->height - 1)
  {
    btn->state = BUTTON_STATE_PRESSED; // Set to pressed state
    return true;
  }

  return false;
}

/**
 * @name       :void GUI_ResetButtonState(Button *btn)
 * @function   :Reset button state to normal
 * @parameters :btn:Button pointer
 * @retvalue   :None
 */
void GUI_ResetButtonState(Button *btn)
{
  if (btn && btn->state == BUTTON_STATE_PRESSED)
  {
    btn->state = BUTTON_STATE_NORMAL;
  }
}

/**
 * @name       :void TEST_ButtonDemo(void)
 * @date       :2023-12-08
 * @function   :Button control demonstration (with inversion effect)
 * @parameters :None
 * @retvalue   :None
 */
void TEST_ButtonDemo(void)
{
  // Create three buttons
  Button btn1 = {20, 35, 20, 20, (u8 *)"1", 16, BUTTON_STATE_NORMAL, true};
  Button btn2 = {50, 35, 20, 20, (u8 *)"2", 16, BUTTON_STATE_NORMAL, true};
  Button btn3 = {80, 35, 20, 20, (u8 *)"3", 16, BUTTON_STATE_DISABLED, true};

  // Display title
  GUI_ShowString(30, 0, (u8 *)"BUTTON DEMO", 16, 1);
  GUI_DrawLine(0, 15, WIDTH - 1, 15, 1);

  // Initial draw all buttons
  GUI_DrawButton(&btn1);
  GUI_DrawButton(&btn2);
  GUI_DrawButton(&btn3);

  OLED_update();
  delay(1000);
  ESP.wdtFeed();

  // Simulate button 1 click - Show inversion effect
  // Use button's own properties to calculate click position (center of the button)
  GUI_CheckButtonPress(&btn1, btn1.x + btn1.width / 2, btn1.y + btn1.height / 2);
  GUI_DrawButton(&btn1); // Redraw button to show inversion effect
  OLED_update();
  delay(500);
  ESP.wdtFeed();

  // Release button 1 - Restore normal display
  GUI_ResetButtonState(&btn1);
  GUI_DrawButton(&btn1);
  OLED_update();
  delay(500);
  ESP.wdtFeed();

  // Simulate button 2 click - Show inversion effect
  // Use button's own properties to calculate click position (center of the button)
  GUI_CheckButtonPress(&btn2, btn2.x + btn2.width / 2, btn2.y + btn2.height / 2);
  GUI_DrawButton(&btn2);
  OLED_update();
  delay(500);
  ESP.wdtFeed();

  // Release button 2
  GUI_ResetButtonState(&btn2);
  GUI_DrawButton(&btn2);
  OLED_update();
  delay(500);
  ESP.wdtFeed();

  // Try clicking disabled button (no effect)
  // Use button's own properties to calculate click position (center of the button)
  bool clicked = GUI_CheckButtonPress(&btn3, btn3.x + btn3.width / 2, btn3.y + btn3.height / 2);
  GUI_DrawButton(&btn3);
  OLED_update();
  delay(1000);
  ESP.wdtFeed();

  // Additional demonstration: State switching
  GUI_Fill(0, 0, WIDTH - 1, HEIGHT - 1, 0);
  GUI_ShowString(10, 0, (u8 *)"STATE CHANGE", 16, 1);
  GUI_DrawLine(0, 15, WIDTH - 1, 15, 1);

  // Enable disabled button
  btn3.state = BUTTON_STATE_NORMAL;
  btn3.text = (u8 *)"Enabled";

  // Draw and update
  GUI_DrawButton(&btn1);
  GUI_DrawButton(&btn2);
  GUI_DrawButton(&btn3);
  OLED_update();
  delay(1000);
  ESP.wdtFeed();

  // Press two buttons simultaneously, show inversion effect
  // Use buttons' own properties to calculate click positions (center of the buttons)
  GUI_CheckButtonPress(&btn1, btn1.x + btn1.width / 2, btn1.y + btn1.height / 2);
  GUI_CheckButtonPress(&btn3, btn3.x + btn3.width / 2, btn3.y + btn3.height / 2);
  GUI_DrawButton(&btn1);
  GUI_DrawButton(&btn3);
  OLED_update();
  delay(1000);
  ESP.wdtFeed();
}