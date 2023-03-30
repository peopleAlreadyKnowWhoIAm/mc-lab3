#include "event-manager.h"

#define ev_div (256 / EVENT_BUFFER)

static uint8_t cycle = 0;

static volatile Event event_list[EVENT_BUFFER];

uint8_t incptr(uint8_t ptr) {
  ptr++;
  if (ptr == EVENT_BUFFER) {
    ptr = 0;
  }
  return ptr;
}

void initEventManager() {
  initTimer1(EVENT_PRESCALLER, EVENT_CYCLE_PERIOD, handleTimer);
}

int8_t spawnEvent(Event event) {
  // Ignore overflow
  event.delay += cycle;

  uint8_t pos = event.delay / ev_div;
  uint8_t counter = EVENT_BUFFER - 1;
  while (counter--) {
    if (event_list[pos].func == 0) {
      event_list[pos] = event;
      pos = incptr(pos);
      return 0;
    }
    pos = incptr(pos);
  }
  return -1;
}

void executeEvent() {
  // Find pos at which the task must be for this cycle
  uint8_t pos = cycle / ev_div;
  // Do for all tasks from that pos to first empty gap or out of space
  while (event_list[pos].func != 0) {
    // Execute function if there is correct for this time
    if (event_list[pos].delay == cycle) {
      void (*func)() =  event_list[pos].func;
      event_list[pos].func = 0;
      func();
    } else {
      // Find place in which the task has to be
      uint8_t desirable_pos = event_list[pos].delay / ev_div;
      // Check whether it isn't in place, if is do nothing
      if (desirable_pos != pos) {
        // Find empty place earlier
        while (event_list[desirable_pos].func != 0 && desirable_pos != pos) {
          desirable_pos = incptr(desirable_pos);
        }
        // If the task not encounter itself
        if (desirable_pos != pos) {
          // Shift here
          event_list[desirable_pos] = event_list[pos];
          event_list[pos].func = 0;
        } else {
          // There aren't any empty space between, so execution for next don't
          // have any sense.
          break;
        }
      }
    }
    pos = incptr(pos);
  }
}

void handleTimer() {
  cycle++;
  executeEvent();
}
