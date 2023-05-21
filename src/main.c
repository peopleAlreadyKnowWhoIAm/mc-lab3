#include "main.h"

#include <stdlib.h>

char text[33];

void setup();

int main() {
  setup();
  while (1) {
  }
  return 0;
}

void setup() {
  // Display data
  DDRD = 0xff;
  // Display:
  // PB0 - E
  // PB1 - RW
  // PB2 - RS
  // Keyboard
  // PB3 - O y0
  // PB4 - O y1
  // PB5 - O y2
  DDRB = 0b111111;

  // Keyboard:
  // PC0 - O y3
  // PC1-PC4 - I PU x0-x4
  // PC5 - buzzer
  DDRC = 0b100001;
  // PORTC |= 0b11110;

  ClockManagerInit();
  sei();
}
