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

/**
 * @name       :void GUI_DrawButton(Button *btn)
 * @function   :Draw button (supports both text and image buttons)
 * @parameters :btn:Button pointer
 * @retvalue   :None
 */
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
    // Pressed state: Button background color becomes GUI background color, text/image color becomes GUI foreground color
    buttonBackgroundColor = guiBackgroundColor;
    buttonTextColor = guiForegroundColor;
    buttonBorderColor = guiForegroundColor;
  }
  else if (btn->state == BUTTON_STATE_DISABLED)
  {
    // Disabled state: Light gray background (implemented with dots)
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

    // Handle text or image drawing for disabled state
    if (btn->type == BUTTON_TYPE_TEXT && btn->text != NULL)
    {
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
            // rowData = F8X16[(currentChar - ' ') * 16 + i];
            rowData = pgm_read_byte(&Font8X16[(currentChar - ' ') * 16 + i]);
          }
          else
          {
            // rowData = F6x8[currentChar - ' '][i];
            rowData = pgm_read_byte(&Font6x8[currentChar - ' '][i]);
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
    }
    else if (btn->type == BUTTON_TYPE_IMAGE && btn->imageData != NULL)
    {
      // For disabled image buttons, draw the image with gray effect
      // Draw image at center of button
      u8 imageX = btn->x + (btn->width - btn->width) / 2;
      u8 imageY = btn->y + (btn->height - btn->height) / 2;

      // Draw image with inversion for disabled effect
      GUI_DrawBMP(imageX, imageY, btn->width, btn->height, btn->imageData, !guiBackgroundColor);
    }
    return; // Already processed disabled state, return directly
  }
  else
  {
    // Normal state: Button background color is opposite to GUI background color, text/image color is the same as GUI background color
    buttonBackgroundColor = guiForegroundColor;
    buttonTextColor = guiBackgroundColor;
    buttonBorderColor = guiBackgroundColor;
  }

  // Draw button background
  GUI_FillRectangle(btn->x, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, buttonBackgroundColor);

  // Draw button border
  GUI_DrawRectangle(btn->x, btn->y, btn->x + btn->width - 1, btn->y + btn->height - 1, buttonBorderColor);

  // Draw button content based on type
  if (btn->type == BUTTON_TYPE_TEXT && btn->text != NULL)
  {
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
  else if (btn->type == BUTTON_TYPE_IMAGE && btn->imageData != NULL)
  {
    // Draw image at center of button
    u8 imageX = btn->x + (btn->width - btn->width) / 2;
    u8 imageY = btn->y + (btn->height - btn->height) / 2;

    // For pressed state, invert the image colors
    u8 imageMode = (btn->state == BUTTON_STATE_PRESSED) ? !guiBackgroundColor : guiBackgroundColor;
    GUI_DrawBMP(imageX, imageY, btn->width, btn->height, btn->imageData, imageMode);
  }
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
 * @name       :Button *createDynamicButton(u8 x, u8 y, u8 width, u8 height,
 *                            u8 *text, u8 textSize, u8 *imageData, ButtonType type,
 *                            ButtonState state, bool visible)
 * @function   :Create a dynamic button
 * @parameters :x:Button x coordinate
 *              y:Button y coordinate
 *              width:Button width
 *              height:Button height
 *              text:Button text
 *              textSize:Button text size
 *              imageData:Button image data
 *              type:Button type
 *              state:Button state
 *              visible:Button visibility
 * @retvalue   :Button pointer
 */
Button *createDynamicButton(u8 x, u8 y, u8 width, u8 height,
                            u8 *text, u8 textSize, u8 *imageData, ButtonType type,
                            ButtonState state, bool visible)
{
  Button *btn = (Button *)malloc(sizeof(Button));
  if (btn == NULL)
  {
    Serial.println("Failed to allocate memory for button!");
    return NULL;
  }

  btn->x = x;
  btn->y = y;
  btn->width = width;
  btn->height = height;
  btn->text = text;
  btn->textSize = textSize;
  btn->imageData = imageData;
  btn->type = type;
  btn->state = state;
  btn->visible = visible;

  return btn;
}

/**
 * @name       :void freeButton(Button **btnPtr)
 * @function   :Free memory of a button
 * @parameters :btnPtr:Button pointer pointer
 * @retvalue   :None
 */
void freeButton(Button **btnPtr)
{
  if (btnPtr && *btnPtr)
  {
    free(*btnPtr);
    *btnPtr = NULL; // Avoid dangling pointer
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
  // Create three buttons (backward compatible)
  Button btn1 = {20, 35, 20, 20, (u8 *)"1", 16, NULL, BUTTON_TYPE_TEXT, BUTTON_STATE_NORMAL, true};
  Button btn2 = {50, 35, 20, 20, (u8 *)"2", 16, NULL, BUTTON_TYPE_TEXT, BUTTON_STATE_NORMAL, true};
  Button btn3 = {80, 35, 20, 20, (u8 *)"3", 16, NULL, BUTTON_TYPE_TEXT, BUTTON_STATE_DISABLED, true};

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
  GUI_CheckButtonPress(&btn3, btn3.x + btn3.width / 2, btn3.y + btn3.height / 2);
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

/**
 * @name       :void TEST_ImageButtonDemo(void)
 * @function   :Image button demonstration
 * @parameters :None
 * @retvalue   :None
 */
void TEST_ImageButtonDemo(void)
{
  // Create image buttons using existing BMP data from bmp.h
  // Note: Make sure you have BMP data defined in bmp.cpp or included properly

  Button imgBtn1 = {10, 30, 16, 16, NULL, 0, (u8 *)BMP2, BUTTON_TYPE_IMAGE, BUTTON_STATE_NORMAL, true};
  Button imgBtn2 = {40, 30, 16, 16, NULL, 0, (u8 *)BMP3, BUTTON_TYPE_IMAGE, BUTTON_STATE_NORMAL, true};
  Button imgBtn3 = {70, 30, 16, 16, NULL, 0, (u8 *)BMP4, BUTTON_TYPE_IMAGE, BUTTON_STATE_DISABLED, true};

  // Create a text button for comparison
  Button textBtn = {100, 30, 25, 16, (u8 *)"TXT", 8, NULL, BUTTON_TYPE_TEXT, BUTTON_STATE_NORMAL, true};

  // Display title
  GUI_ShowString(20, 0, (u8 *)"IMAGE BUTTON DEMO", 16, 1);
  GUI_DrawLine(0, 15, WIDTH - 1, 15, 1);

  // Draw all buttons
  GUI_DrawButton(&imgBtn1);
  GUI_DrawButton(&imgBtn2);
  GUI_DrawButton(&imgBtn3);
  GUI_DrawButton(&textBtn);

  OLED_update();
  delay(2000);
  ESP.wdtFeed();

  // Simulate clicking image button 1
  GUI_CheckButtonPress(&imgBtn1, imgBtn1.x + imgBtn1.width / 2, imgBtn1.y + imgBtn1.height / 2);
  GUI_DrawButton(&imgBtn1);
  OLED_update();
  delay(1000);
  ESP.wdtFeed();

  // Release image button 1
  GUI_ResetButtonState(&imgBtn1);
  GUI_DrawButton(&imgBtn1);
  OLED_update();
  delay(500);
  ESP.wdtFeed();

  // Simulate clicking image button 2
  GUI_CheckButtonPress(&imgBtn2, imgBtn2.x + imgBtn2.width / 2, imgBtn2.y + imgBtn2.height / 2);
  GUI_DrawButton(&imgBtn2);
  OLED_update();
  delay(1000);
  ESP.wdtFeed();

  // Release image button 2
  GUI_ResetButtonState(&imgBtn2);
  GUI_DrawButton(&imgBtn2);
  OLED_update();
  delay(500);
  ESP.wdtFeed();

  // Try clicking disabled image button (no effect)
  GUI_CheckButtonPress(&imgBtn3, imgBtn3.x + imgBtn3.width / 2, imgBtn3.y + imgBtn3.height / 2);
  GUI_DrawButton(&imgBtn3);
  OLED_update();
  delay(1000);
  ESP.wdtFeed();
}

/**
 * @name       :void gui_draw_btn_pic(uint8_t x, uint8_t y, const uint8_t *bmp)
 * @function   :Draw a 48x48 BMP image on OLED
 * @parameters :x - X coordinate of top-left corner
 *              y - Y coordinate of top-left corner
 *              bmp - Pointer to BMP image data
 * @retvalue   :None
 */
void gui_draw_btn_pic(uint8_t x, uint8_t y, const uint8_t *bmp)
{
  u16 sx, sy;
  for (sy = 0; sy < 48; sy++)
  {
    for (sx = 0; sx < 48; sx++)
    {
      OLED_Set_Pixel(x + sx, y + sy, (pgm_read_byte(&bmp[sy * 48 + sx]) > 0x2f) ? 1 : 0);
    }
  }
}
