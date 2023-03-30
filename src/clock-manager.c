#include "clock-manager.h"

// TODO
// - Complete buzzer signal
// - Add buzzer every minute

void _rewriteDisplay();
void _processStopwatch();
void _clear(volatile char* text, uint8_t size);

void ClockManagerInit() {
  initEventManager();
  DisplayInit();
  KeyboardInit();
  _processStopwatch();
  spawnEvent((Event){.delay = 50, .func = _rewriteDisplay});
}

volatile static struct {
  uint8_t running : 1;
  uint8_t blinking : 1;
  uint8_t memory_mode : 1;
  uint8_t started_entering : 1;
} state;
// "    hh:mm:ss.s"
volatile static char line[34] = "                \n                 ";

volatile static uint32_t time = 0;

volatile static uint32_t time_memory[MEMORY_BUFFER_SIZE];
volatile static uint8_t time_memory_pos = 0;

volatile static uint8_t time_memory_show = 0;

volatile static uint8_t minutes_last = 0;

void KeyboardWriteLine(uint8_t line) {
  const uint8_t b_mask = 0b111000;
  const uint8_t c_mask = 0b1;
  PORTB &= ~b_mask;
  PORTC &= ~c_mask;
  PORTB |= (b_mask & (line << 3));
  PORTC |= (c_mask & (line >> 3));
}

uint8_t KeyboardReadRows() {
  uint8_t a = ((PINC >> 1) & 0b1111);
  return a;
}

int8_t calculatePresedNum(uint8_t x, uint8_t y) {
  if (x == 1 && y == 3) {
    return 0;
  }
  if (x >= 3 || y >= 3) {
    if (y == 3) {
      const uint8_t vals[] = {0, 0, 1, 2};
      return -y - vals[x] - 1;
    }
    return -y - 1;
  }
  return (2 - y) * 3 + x + 1;
}

void toClockNum(uint8_t num, volatile char* placement) {
  placement[1] = num % 10 + '0';
  placement[0] = num / 10 + '0';
}

void _convertToDigits(uint32_t calc_time, uint8_t* digits_place) {
  digits_place[3] = calc_time % 10;
  calc_time /= 10;
  digits_place[2] = (calc_time) % 60;
  calc_time /= 60;
  digits_place[1] = calc_time % 60;
  calc_time /= 60;
  digits_place[0] = calc_time;
}

void _formTimeString(volatile char* placement, uint8_t* digits) {
  toClockNum(digits[0], &placement[0]);
  placement[2] = ':';
  toClockNum(digits[1], &placement[3]);
  placement[5] = ':';
  toClockNum(digits[2], &placement[6]);
  placement[8] = '.';
  placement[9] = '0' + digits[3];
}

void _processStopwatch() {
  uint8_t digits[4];

  _convertToDigits(time, digits);

  if (minutes_last != digits[1]) {
    minutes_last = digits[1];
    BuzzerLongSignal();
  }

  _formTimeString(&line[0], digits);

  if (state.running) {
    time++;
    spawnEvent((Event){.delay = 100, .func = _processStopwatch});
  }
}

void _blinkContent() {
  static bool showing = true;
  if (showing && state.blinking) {
    showing = false;
    _clear(&line[0], 10);
    spawnEvent((Event){.delay = 200, .func = _blinkContent});
  } else {
    uint8_t digits[4];

    _convertToDigits(time, digits);

    _formTimeString(&line[0], digits);
    showing = true;
    if (state.blinking) {
      spawnEvent((Event){.delay = 200, .func = _blinkContent});
    }
  }
}

void _rewriteMemorySign() {
  if (time_memory_pos == 0) {
    line[14] = ' ';
    line[15] = ' ';
    line[13] = ' ';
  } else {
    line[13] = 'M';
    toClockNum(time_memory_pos, &line[14]);
  }
}

void _clear(volatile char* first_symbol, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    first_symbol[i] = ' ';
  }
}

void _clearLine(volatile char* first_symbol) { _clear(first_symbol, 16); }

void _rewriteMemoryLine() {
  if (state.memory_mode && time_memory_pos != 0) {
    if (time_memory_show >= time_memory_pos) {
      time_memory_show = 0;
    }
    line[18] = '0' + (time_memory_show + 1) % 10;
    if (time_memory_show + 1 >= 10) {
      line[17] = '0' + (time_memory_show + 1) / 10;
    } else {
      line[17] = ' ';
    }
    line[19] = '.';
    uint8_t digits[4];

    _convertToDigits(time_memory[time_memory_show], digits);

    _formTimeString(&line[21], digits);

  } else {
    _clearLine(&line[17]);
  }
}

void _rewriteDisplay() {
  DisplayRewrite((char*)line);
  spawnEvent((Event){.delay = 100, .func = _rewriteDisplay});
}

void KeyboardProcessClick(uint8_t x, uint8_t y) {
  // itoa(calculatePresedNum(x,y), txt, 10);

  int8_t button = calculatePresedNum(x, y);
  ProcessButton(button);
}
void ProcessButton(int8_t button) {
  static volatile int8_t last_digit;
  // Start/Pause stopwatch
  if (button == ButtonD) {
    if (state.running) {
      state.running = false;
      state.blinking = true;
      spawnEvent((Event){.delay = 199, .func = _blinkContent});

    } else {
      state.blinking = false;
      state.running = true;
      spawnEvent((Event){.delay = 50, .func = _processStopwatch});
    }
    BuzzerShortSignal();
  }
  // Clear time
  else if (button == ButtonC) {
    BuzzerShortSignal();
    time = 0;
    state.blinking = false;
    state.running = false;
    uint8_t digits[4];

    _convertToDigits(time, digits);

    _formTimeString(&line[0], digits);
  }
  // Clear memory
  else if (button == ButtonAstr) {
    if (state.started_entering) {
      state.started_entering = false;
      BuzzerLongSignal();
      _rewriteMemoryLine();
    } else {
      BuzzerShortSignal();
      time_memory_pos = 0;
      state.memory_mode = false;
      _rewriteMemorySign();
      _rewriteMemoryLine();
    }
  }
  // Confirm entering/ Save to memory
  else if (button == ButtonHash) {
    if (state.started_entering) {
      time_memory_show = last_digit - 1;
      if (time_memory_show >= time_memory_pos || time_memory_pos == 0) {
        time_memory_show = 0;
        BuzzerLongSignal();
      } else {
        BuzzerDoubleSignal();
      }
      state.started_entering = false;
      _rewriteMemoryLine();
    } else if (time_memory_pos < MEMORY_BUFFER_SIZE &&
               (time_memory_pos == 0 || time_memory[time_memory_pos] != time)) {
      BuzzerShortSignal();
      time_memory[time_memory_pos] = time;
      time_memory_pos++;
      _rewriteMemorySign();
    }
  }
  // Toggle memory mode
  else if (button == ButtonA) {
    BuzzerShortSignal();
    state.memory_mode = !state.memory_mode;
    _rewriteMemoryLine();

  }
  // Scroll through memory
  else if (button == ButtonB && state.memory_mode) {
    BuzzerShortSignal();
    time_memory_show++;
    if (time_memory_show == time_memory_pos) {
      time_memory_show = 0;
    }
    _rewriteMemoryLine();
  }
  // Enter numeber of a memory storage
  else if (button >= 0 && state.memory_mode) {
    if (!state.started_entering) {
      BuzzerShortSignal();
      last_digit = button;
      _clearLine(&line[17]);
      line[17] = button + '0';
    } else {
      time_memory_show = last_digit * 10 + button - 1;
      if (time_memory_show >= time_memory_pos || time_memory_pos == 0) {
        time_memory_show = 0;
        BuzzerLongSignal();
      } else {
        BuzzerDoubleSignal();
      }
      _rewriteMemoryLine();
    }
    state.started_entering = !state.started_entering;
  }
}