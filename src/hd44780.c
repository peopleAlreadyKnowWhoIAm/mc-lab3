#include "hd44780.h"

#define CLEAR_CONTROL (MANAGING_PORT &= ~((MANAGING_RS_Mask | MANAGING_E_Mask)))

static const uint8_t lines_addr[] = {
    0,
    0x40,
};

static const uint8_t start_sequence[] = {
    DISPLAY_FUNCTION | DISPLAY_FUNCTION_DATALENGTH8 | DISPLAY_FUNCTION_2_LINES,
    DISPLAY_POWER_CONTROL | DISPLAY_POWER_CONTROL_ON,
    DISPLAY_ENTRY_MODE | DISPLAY_ENTRY_MODE_INCREMENT,
};

static volatile bool busy = true;

static volatile uint8_t ddram_addr = 0;

void _resumeDisplay() {

  MANAGING_PORT &= ~MANAGING_E_Mask;
}

void _clearBusy() {
  busy = false;
  // MANAGING_PORT |= MANAGING_RS_Mask;

  // DisplayWrite("Hello World!");
  // MANAGING_PORT &= ~MANAGING_E_Mask;
  // DATA_PORT = 'h';
  // MANAGING_PORT |= MANAGING_E_Mask | MANAGING_RS_Mask;
}

void _subDisplayInit() {
  static volatile uint8_t counter = 0;
  DATA_PORT = start_sequence[counter];
  MANAGING_PORT |= MANAGING_E_Mask;
  counter++;
  if (counter != 3) {
    uint8_t delay = DISPLAY_WAIT;
    if (counter == 1) {
      delay = DISPLAY_INIT_TIME;
    }
    spawnEvent((Event){.delay = delay, .func = _subDisplayInit});
    MANAGING_PORT &= ~MANAGING_E_Mask;
  } else {
    spawnEvent(
        (Event){.delay = DISPLAY_WAIT, .func = _clearBusy});
  }
  _resumeDisplay();
}

void DisplayInit() {
  spawnEvent((Event){.delay = DISPLAY_ON_TIME, .func = _subDisplayInit});
}
volatile char* data;

void _subDisplayWrite() {
  if (*data == '\n') {
    ddram_addr = lines_addr[1];
    DATA_PORT = DISPLAY_DDRAM_ADDR | (0b1111111 & ddram_addr);
    MANAGING_PORT |= MANAGING_E_Mask;
    spawnEvent((Event){.delay = DISPLAY_WAIT, .func = _subDisplayWrite});
    MANAGING_PORT &= ~MANAGING_E_Mask;

    data++;
  } else if (*data != 0) {
    MANAGING_PORT |= MANAGING_RS_Mask;
    // MANAGING_PORT |= MANAGING_RS_Mask;
    DATA_PORT = *data;
    data++;
    ddram_addr++;
    MANAGING_PORT |= MANAGING_E_Mask;
    spawnEvent((Event){.delay = DISPLAY_WAIT, .func = _subDisplayWrite});
    MANAGING_PORT &= ~MANAGING_E_Mask;
    MANAGING_PORT &= ~MANAGING_RS_Mask;
  } else {
    busy = false;
  }
}

int8_t DisplayWrite(char* text) {
  if (busy) {
    return -1;
  }
  busy = true;
  data = text;
  spawnEvent((Event){.delay = DISPLAY_WAIT, .func = _subDisplayWrite});
  return 0;
}

void _subDisplaySetCursor() {
  MANAGING_PORT |= MANAGING_E_Mask;
  DATA_PORT = DISPLAY_DDRAM_ADDR | (0b1111111 & ddram_addr);
  _resumeDisplay();
}

int8_t DisplaySetCursor(uint8_t x, uint8_t y) {
  if (busy) {
    return -1;
  }
  busy = true;
  ddram_addr = lines_addr[y] + x;
  _subDisplaySetCursor();
  spawnEvent(
      (Event){.delay = DISPLAY_WAIT, .func = _clearBusy});
  return 0;
}

int8_t DisplaySetCursorHome() { return DisplaySetCursor(0, 0); }

int8_t DisplayClear() {
  if (busy) {
    return -1;
  }
  busy = true;
  DATA_PORT = DISPLAY_CLEAR;
  MANAGING_PORT |= MANAGING_E_Mask;
  spawnEvent(
      (Event){.delay = DISPLAY_ON_TIME, .func = _clearBusy});
  _resumeDisplay();
  return 0;
}

int8_t DisplayRewriteLine(uint8_t line, char* text) {
  if (busy) {
    return -1;
  }
  busy = true;
  ddram_addr = lines_addr[line];
  data = text;
  _subDisplaySetCursor();
  spawnEvent((Event){.delay = DISPLAY_WAIT, .func = _subDisplayWrite});

  return 0;
}

int8_t DisplayRewrite(char* text){
  return DisplayRewriteLine(0, text);
}