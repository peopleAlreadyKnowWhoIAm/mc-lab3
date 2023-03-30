#ifndef _MAIN_H_
#define _MAIN_H_
#define KEYBOARD_LINES 4
#define KEYBOARD_ROWS 4

#include <avr/io.h>

#include <avr/interrupt.h>

#include "event-manager.h"
#include "hd44780.h"
#include "matrix-keyboard.h"
#include "clock-manager.h"



int main();

#endif