#include "../inc/includes.h"

/*
 * @brief Create a text edit widget
 *
 * This function creates a text edit widget with the specified position, size, and maximum length.
 *
 * @param x The x-coordinate of the widget's top-left corner
 * @param y The y-coordinate of the widget's top-left corner
 * @param width The width of the widget
 * @param height The height of the widget
 * @param maxLength The maximum length of the editable text
 * @return TextEditWidget* A pointer to the created text edit widget, or NULL if creation fails
 */
TextEditWidget *createTextEdit(u8 x, u8 y, u8 width, u8 height, u8 maxLength)
{
  TextEditWidget *widget = (TextEditWidget *)malloc(sizeof(TextEditWidget));
  if (widget)
  {
    widget->x = x;
    widget->y = y;
    widget->width = width;
    widget->height = height;
    widget->maxLength = maxLength;
    widget->currentLength = 0;
    widget->cursorPos = 0;
    widget->editMode = false;
    widget->buffer = (u8 *)malloc(maxLength + 1);
    if (widget->buffer)
    {
      widget->buffer[0] = '\0';
    }
    else
    {
      free(widget);
      widget = NULL;
    }
  }
  return widget;
}

/*
 * @brief Free a text edit widget
 *
 * This function frees the memory allocated for a text edit widget.
 *
 * @param widget A pointer to the text edit widget to be freed
 */
void freeTextEdit(TextEditWidget *widget)
{
  if (widget)
  {
    if (widget->buffer)
    {
      free(widget->buffer);
    }
    free(widget);
  }
}

/*
 * @brief Draw a text edit widget
 *
 * This function draws a text edit widget on the OLED display.
 *
 * @param widget A pointer to the text edit widget to be drawn
 */
void drawTextEdit(TextEditWidget *widget)
{
  GUI_DrawRectangle(widget->x, widget->y, widget->x + widget->width - 1, widget->y + widget->height - 1, 1);

  if (widget->editMode)
  {
    GUI_Fill(widget->x + 1, widget->y + 1, widget->x + widget->width - 2, widget->y + widget->height - 2, 1);
    GUI_ShowString(widget->x + 3, widget->y + (widget->height - 8) / 2, widget->buffer, FONT_6X8, 0);

    static u32 lastBlinkTime = 0;
    static bool cursorVisible = true;
    u32 currentTime = millis();
    if (currentTime - lastBlinkTime > 500)
    {
      cursorVisible = !cursorVisible;
      lastBlinkTime = currentTime;
    }

    if (cursorVisible)
    {
      u8 cursorX = widget->x + 3 + (widget->cursorPos * 6);
      u8 cursorY = widget->y + (widget->height - 8) / 2;
      GUI_Fill(cursorX, cursorY, cursorX, cursorY + 7, 0);
    }
  }
  else
  {
    GUI_ShowString(widget->x + 3, widget->y + (widget->height - 8) / 2, widget->buffer, FONT_6X8, 1);
  }
}

/*
 * @brief Handle input for a text edit widget
 *
 * This function handles input for a text edit widget, such as clicking or holding buttons.
 *
 * @param widget A pointer to the text edit widget to handle input for
 * @param btn The button value (BTN_CLICK or BTN_HOLD)
 */
void handleTextEditInput(TextEditWidget *widget, btnval_t btn)
{
  if (!widget->editMode)
  {
    if (btn == BTN_CLICK)
    {
      widget->editMode = true;
    }
    return;
  }

  switch ((btnval_t)btn)
  {
  case BTN_CLICK:
    static u8 currentChar = 'A';
    if (widget->currentLength < widget->maxLength)
    {
      memmove(&widget->buffer[widget->cursorPos + 1], &widget->buffer[widget->cursorPos], widget->currentLength - widget->cursorPos + 1);
      widget->buffer[widget->cursorPos] = currentChar;
      widget->currentLength++;
      widget->cursorPos++;

      currentChar = (currentChar >= 'Z') ? 'A' : currentChar + 1;
    }
    break;

  case BTN_HOLD:
    if (widget->currentLength > 0 && widget->cursorPos > 0)
    {
      memmove(&widget->buffer[widget->cursorPos - 1], &widget->buffer[widget->cursorPos], widget->currentLength - widget->cursorPos + 1);
      widget->currentLength--;
      widget->cursorPos--;
    }
    break;

  default:
    break;
  }
}

/*
 * @brief Exit text edit mode for a widget
 *
 * This function exits text edit mode for a widget, disabling editing and updating the display.
 *
 * @param widget A pointer to the text edit widget to exit edit mode for
 */
void exitTextEditMode(TextEditWidget *widget)
{
  widget->editMode = false;
}
