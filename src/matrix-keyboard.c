#include "matrix-keyboard.h"

static volatile uint8_t seen_click[KEYBOARD_LINES];
static volatile uint8_t done_action[KEYBOARD_LINES];

void _listenButtons() {
  for (uint8_t y = 0; y < KEYBOARD_LINES; y++) {
    KeyboardWriteLine(~(1 << y));
    _delay_loop_1(2);
    uint8_t readed = ~KeyboardReadRows();
    for (uint8_t x = 0; x < KEYBOARD_ROWS; x++) {
      uint8_t x_mask = 1 << x;
      if ((x_mask & readed)) {
        if (x_mask & seen_click[y] && !(done_action[y] & x_mask)) {
          KeyboardProcessClick(x, y);
          done_action[y] |= x_mask;
        } else {
          seen_click[y] |= x_mask;
        }

      } else {
        if((x_mask & seen_click[y])) {
          done_action[y] &= ~x_mask;
        } else {
          seen_click[y] &= ~x_mask;
        }
      }
    }
  }
  KeyboardWriteLine(0xff);
  spawnEvent((Event) {.delay = 50, .func = _listenButtons});
}

void KeyboardInit() {
  _listenButtons();
}
