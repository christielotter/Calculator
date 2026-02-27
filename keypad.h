#ifndef _KEYPAD_H_
#define _KEYPAD_H_
#include <stdint.h>
#include <stdbool.h>

uint8_t ReadRows(void);
char KeypadRead(void);
extern volatile bool shift_active;
bool ShiftActive(void);
#endif

