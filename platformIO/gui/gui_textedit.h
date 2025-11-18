#ifndef _GUI_TEXTEDIT_H_
#define _GUI_TEXTEDIT_H_

// Text edit widget structure
typedef struct
{
  u8 x;             // X coordinate of the text box top-left corner
  u8 y;             // Y coordinate of the text box top-left corner
  u8 width;         // Width of the text box
  u8 height;        // Height of the text box
  u8 maxLength;     // Maximum number of characters
  u8 currentLength; // Current number of characters
  u8 cursorPos;     // Cursor position
  u8 *buffer;       // Text buffer
  bool editMode;    // Whether it's in edit mode
} TextEditWidget;

TextEditWidget *createTextEdit(u8 x, u8 y, u8 width, u8 height, u8 maxLength);
void freeTextEdit(TextEditWidget *widget);
void drawTextEdit(TextEditWidget *widget);
void handleTextEditInput(TextEditWidget *widget, btnval_t btn);
void exitTextEditMode(TextEditWidget *widget);
#endif
