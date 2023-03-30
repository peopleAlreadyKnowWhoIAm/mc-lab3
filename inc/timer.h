/**
 * @file timer.h
 * @brief File for managing timers
 * Currently manage timer1 only in CTC mode
 * @version 0.1
 * @date 2023-03-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _TIMER_H_
#define _TIMER_H_

#include <avr/io.h>

enum TimerPrescaller {
  TimerPrescaller_off =0b0,
  TimerPrescaller_1 = 0b001,
  TimerPrescaller_8 = 0b010,
  TimerPrescaller_64 = 0b011,
  TimerPrescaller_256 = 0b100,
  TimerPrescaller_1024 = 0b101,
  TimerPrescaller_external_falling = 0b110,
  TimerPrescaller_external_rising = 0b111,

};

/**
 * @brief Init timer in CTC mode
 * 
 */
void initTimer1(enum TimerPrescaller prescaller, uint16_t ticks, void (*handler)());

#endif