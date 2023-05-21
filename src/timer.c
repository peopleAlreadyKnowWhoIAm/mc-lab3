#include "timer.h"

#include <avr/interrupt.h>

static void (*timer_handler)();

ISR(TIMER1_COMPA_vect) { timer_handler(); }

void initTimer1(enum TimerPrescaller prescaller, uint16_t period,
                void (*handler)()) {
  timer_handler = handler;
  // Config timer to CTC mode with OCSR1A
  TCCR1A = 0;
  TCCR1B = _BV(WGM12) | prescaller;

  OCR1A = period;

  TIMSK1 = _BV(OCIE1A);
}