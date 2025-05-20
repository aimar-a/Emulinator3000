#include "chip8_timers.h"

uint8_t *delay_timer;
uint8_t *sound_timer;

void chip8timersInit(uint8_t *delay, uint8_t *sound)
{
  delay_timer = delay;
  *delay_timer = 0;
  sound_timer = sound;
  *sound_timer = 0;
  chip8_log("INFO: Timers initialized with delay and sound set to 0.\n");
  // chip8timersInitAudio();
}

void chip8timersSetDelay(uint8_t value)
{
  *delay_timer = value;
  chip8_log("INFO: Delay timer set to %d.\n", value);
}

void chip8timersSetSound(uint8_t value)
{
  *sound_timer = value;
  chip8_log("INFO: Sound timer set to %d.\n", value);
}

void chip8timersDecrement()
{
  if (*delay_timer > 0)
  {
    (*delay_timer)--;
    chip8_log("INFO: Delay timer decremented to %d.\n", *delay_timer);
  }

  if (*sound_timer > 0)
  {
    (*sound_timer)--;
    chip8_log("INFO: Sound timer decremented to %d.\n", *sound_timer);
  }
}

uint8_t chip8timersGetDelay()
{
  return *delay_timer;
}