#ifndef _CLOCK_MANAGER_H_
#define _CLOCK_MANAGER_H_

#include <avr/io.h>

#include "event-manager.h"
#include "hd44780.h"
#include "buzzer.h"

//Named buttons from -5 to -1
// After digits
enum ButtonDefinition{
  ButtonD = -6,
  ButtonHash,
  ButtonAstr,
  ButtonC,
  ButtonB,
  ButtonA,
};


#include "matrix-keyboard.h"
void KeyboardWriteLine(uint8_t line);
uint8_t KeyboardReadRows();
void KeyboardProcessClick(uint8_t x, uint8_t y);


#define MEMORY_BUFFER_SIZE 60

void ClockManagerInit();

void ProcessButton(int8_t button);


#endif