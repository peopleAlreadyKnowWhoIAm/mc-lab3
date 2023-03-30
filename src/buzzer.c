#include "buzzer.h"

bool busy;

void _enableBuzzer(){
  BUZZER_PORT |= _BV(BUZZER_POS);
}

void _disableBuzzer(){
  BUZZER_PORT &= ~_BV(BUZZER_POS);

}

void _clearBuzzer(){
  _disableBuzzer();
  busy = false;
}


int8_t _buzzerSignal(uint8_t signal){
  if(busy){
    return -1;
  }
  busy = true;
  _enableBuzzer();
  spawnEvent((Event){.delay=signal, .func=_clearBuzzer});
  return 0;

}

int8_t BuzzerShortSignal(){
  return _buzzerSignal(100);
}

int8_t BuzzerLongSignal(){
  return _buzzerSignal(250);
}

int8_t BuzzerDoubleSignal(){
  if(busy){
    return -1;
  }
  busy = true;
  _enableBuzzer();
  spawnEvent((Event){.delay = 100, .func=_disableBuzzer});
  spawnEvent((Event){.delay = 150, .func=_enableBuzzer});
  spawnEvent((Event){.delay = 250, .func=_clearBuzzer});
  return 0;
}