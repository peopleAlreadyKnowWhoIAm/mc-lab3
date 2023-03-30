//WARNING depends on event manager
// WARNING all DDR registers must be configured before using this header file
#ifndef _HD44780_H_
#define _HD44780_H_

#include <stdbool.h>
#include <avr/io.h>

#include "event-manager.h"

#define DISPLAY_ON_TIME 20
#define DISPLAY_INIT_TIME 3
#define DISPLAY_WAIT 1


#define DATA_PORT PORTD
#define MANAGING_PORT PORTB
#define MANAGING_RS_Mask _BV(PB0)
#define MANAGING_RW_Mask _BV(PB1)
#define MANAGING_E_Mask _BV(PB2) //ReadWrite data ToFrom bus FromTo display

void DisplayInit();

int8_t DisplayWrite(char* text);
int8_t DisplayRewriteLine(uint8_t line, char* text);
int8_t DisplayRewrite(char* text);

int8_t DisplaySetCursor(uint8_t x, uint8_t y);

int8_t DisplaySetCursorHome();

int8_t DisplayClear();

// Only 8-bit data pins


#define DISPLAY_CLEAR (0b1)
#define DISPLAY_RETURN_HOME (0b10)
#define DISPLAY_ENTRY_MODE (0b100) //Decrement DDRAM counter
# define DISPLAY_ENTRY_MODE_INCREMENT (0b10) //Increment DDRAM counter
# define DISPLAY_ENTRY_MODE_SHIFT (0b1) //Shift display
#define DISPLAY_POWER_CONTROL (0b1000)
# define DISPLAY_POWER_CONTROL_ON (0b100) //On display
# define DISPLAY_POWER_CONTROL_CURSOR (0b10) //Show cursor
# define DISPLAY_POWER_CONTROL_CURSOR_BLINK (0b1)
#define DISPLAY_SHIFT (0b10000)
# define DISPLAY_SHIFT_ON (0b1000) //Shift display instead of cursor
# define DISPLAY_SHIFT_DIRECTION (0b100) //Shift to the right, else to the left
#define DISPLAY_FUNCTION (0b100000)
# define DISPLAY_FUNCTION_DATALENGTH8 (0b10000)
# define DISPLAY_FUNCTION_2_LINES (0b1000)
# define DISPLAY_FUNCTION_FONT_10 (0b100)
#define DISPLAY_CGRAM_ADDR (0b1000000)
#define DISPLAY_DDRAM_ADDR (0b10000000)

#endif
