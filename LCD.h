#ifndef _LCD_H_
#define _LCD_H_
#include <stdint.h>
void pulseEnable(void);
void SendDisplayNibble(uint8_t nibble, uint8_t rs);
void SendtoDisplay(uint8_t value, uint8_t rs);
void InitDisplayPort(void);
void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);
void clearDisplayScreen(void);
void moveDisplayCursor(uint8_t row, uint8_t col);
void printDisplay(const char *string);
void EraseCharacter (void);
void LCD_PrintFloat(float value);
void LCD_PrintAt(uint8_t row, uint8_t col, char c);
#endif