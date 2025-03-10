#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>

void audioCallback(void *userdata, uint8_t *stream, int len);
void iniciarSonido();
void set_delay_timer(uint8_t value);
void set_sound_timer(uint8_t value);
void decrement_timers();
uint8_t get_delay_timer();
void iniciarSonido();

uint8_t get_sound_timer();

#endif // !TIMERS_H
