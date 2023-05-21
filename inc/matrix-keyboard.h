#ifndef _MATRIX_KEYBOARD_H_
#define _MATRIX_KEYBOARD_H_

#include <stdint.h>
#include <stdbool.h>

#include <util/delay_basic.h>

#include "event-manager.h"

#ifndef KEYBOARD_LINES
# define KEYBOARD_LINES 4
# define KEYBOARD_ROWS 4
// # error "Lines and rows must be predefined"
#endif


//!! The function below must be written by user
void KeyboardWriteLine(uint8_t line);
uint8_t KeyboardReadRows();
void KeyboardProcessClick(uint8_t x, uint8_t y);

void KeyboardInit();

#endif
