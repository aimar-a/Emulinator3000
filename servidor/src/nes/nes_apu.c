#include "nes_apu.h"

// APU register addresses
#define APU_REG_PULSE1_CTRL 0x4000
#define APU_REG_PULSE1_SWEEP 0x4001
#define APU_REG_PULSE1_TIMER_L 0x4002
#define APU_REG_PULSE1_LENGTH 0x4003
#define APU_REG_PULSE2_CTRL 0x4004
#define APU_REG_PULSE2_SWEEP 0x4005
#define APU_REG_PULSE2_TIMER_L 0x4006
#define APU_REG_PULSE2_LENGTH 0x4007
#define APU_REG_TRI_CTRL 0x4008
#define APU_REG_TRI_UNUSED 0x4009
#define APU_REG_TRI_TIMER_L 0x400A
#define APU_REG_TRI_LENGTH 0x400B
#define APU_REG_NOISE_CTRL 0x400C
#define APU_REG_NOISE_UNUSED 0x400D
#define APU_REG_NOISE_PERIOD 0x400E
#define APU_REG_NOISE_LENGTH 0x400F
#define APU_REG_DMC_CTRL 0x4010
#define APU_REG_DMC_DIRECT 0x4011
#define APU_REG_DMC_ADDR 0x4012
#define APU_REG_DMC_LENGTH 0x4013
#define APU_REG_STATUS 0x4015
#define APU_REG_FRAME_COUNTER 0x4017

// Audio settings
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 1024 // en nes_structure.h se define como 1024 manualmente (no bonito)

// Duty cycle patterns for pulse channels
static const uint8_t DUTY_CYCLES[4][8] = {
    {0, 1, 0, 0, 0, 0, 0, 0}, // 12.5%
    {0, 1, 1, 0, 0, 0, 0, 0}, // 25%
    {0, 1, 1, 1, 1, 0, 0, 0}, // 50%
    {1, 0, 0, 1, 1, 1, 1, 1}  // 25% negated
};

// Triangle wave sequence
static const uint8_t TRIANGLE_SEQUENCE[32] = {
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

// Noise period table
static const uint16_t NOISE_PERIODS[16] = {
    4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068};

// Initialize the APU
void nes_apu_init(APU *apu)
{
  memset(apu, 0, sizeof(APU));

  /*
  // Initialize SDL audio subsystem
  if (SDL_Init(SDL_INIT_AUDIO) != 0)
  {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return;
  }

  // Initialize SDL audio
  SDL_AudioSpec desired, obtained;
  desired.freq = SAMPLE_RATE;
  desired.format = AUDIO_F32;
  desired.channels = 1;
  desired.samples = BUFFER_SIZE;
  desired.callback = NULL; // We'll use queue audio

  apu->audio_device = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, 0);
  if (apu->audio_device == 0)
  {
    nes_log_error("ERROR: Failed to open audio device: %s\n", SDL_GetError());
    return;
  }

  SDL_PauseAudioDevice(apu->audio_device, 0);
  */

  // Initialize noise shift register
  apu->noise.shift_register = 1;
}

// Write to an APU register
void nes_apu_write(APU *apu, uint16_t address, uint8_t value)
{
  nes_log_traceback("INFO: APU Writing address: 0x%04X, Value: 0x%02X", address, value);
  switch (address)
  {
  // Pulse 1
  case APU_REG_PULSE1_CTRL:
    apu->pulse1.duty_cycle = (value >> 6) & 0x03;
    apu->pulse1.length_halt = (value >> 5) & 0x01;
    apu->pulse1.constant_volume = (value >> 4) & 0x01;
    apu->pulse1.volume = value & 0x0F;
    break;
  case APU_REG_PULSE1_SWEEP:
    apu->pulse1.sweep_enable = (value >> 7) & 0x01;
    apu->pulse1.sweep_period = (value >> 4) & 0x07;
    apu->pulse1.sweep_negate = (value >> 3) & 0x01;
    apu->pulse1.sweep_shift = value & 0x07;
    break;
  case APU_REG_PULSE1_TIMER_L:
    apu->pulse1.timer_reload = (apu->pulse1.timer_reload & 0xFF00) | value;
    break;
  case APU_REG_PULSE1_LENGTH:
    apu->pulse1.timer_reload = (apu->pulse1.timer_reload & 0x00FF) | ((value & 0x07) << 8);
    apu->pulse1.length_counter = (value >> 3) & 0x1F;
    apu->pulse1.sequencer = 0;
    apu->pulse1.envelope_divider = apu->pulse1.volume;
    apu->pulse1.envelope_counter = 15;
    break;

  // Pulse 2 (similar to pulse1)
  case APU_REG_PULSE2_CTRL:
    apu->pulse2.duty_cycle = (value >> 6) & 0x03;
    apu->pulse2.length_halt = (value >> 5) & 0x01;
    apu->pulse2.constant_volume = (value >> 4) & 0x01;
    apu->pulse2.volume = value & 0x0F;
    break;
  case APU_REG_PULSE2_SWEEP:
    apu->pulse2.sweep_enable = (value >> 7) & 0x01;
    apu->pulse2.sweep_period = (value >> 4) & 0x07;
    apu->pulse2.sweep_negate = (value >> 3) & 0x01;
    apu->pulse2.sweep_shift = value & 0x07;
    break;
  case APU_REG_PULSE2_TIMER_L:
    apu->pulse2.timer_reload = (apu->pulse2.timer_reload & 0xFF00) | value;
    break;
  case APU_REG_PULSE2_LENGTH:
    apu->pulse2.timer_reload = (apu->pulse2.timer_reload & 0x00FF) | ((value & 0x07) << 8);
    apu->pulse2.length_counter = (value >> 3) & 0x1F;
    apu->pulse2.sequencer = 0;
    apu->pulse2.envelope_divider = apu->pulse2.volume;
    apu->pulse2.envelope_counter = 15;
    break;

  // Triangle
  case APU_REG_TRI_CTRL:
    apu->triangle.length_halt = (value >> 7) & 0x01;
    apu->triangle.linear_reload = value & 0x7F;
    break;
  case APU_REG_TRI_TIMER_L:
    apu->triangle.timer_reload = (apu->triangle.timer_reload & 0xFF00) | value;
    break;
  case APU_REG_TRI_LENGTH:
    apu->triangle.timer_reload = (apu->triangle.timer_reload & 0x00FF) | ((value & 0x07) << 8);
    apu->triangle.length_counter = (value >> 3) & 0x1F;
    apu->triangle.linear_reload_flag = true;
    break;

  // Noise
  case APU_REG_NOISE_CTRL:
    apu->noise.length_halt = (value >> 5) & 0x01;
    apu->noise.constant_volume = (value >> 4) & 0x01;
    apu->noise.volume = value & 0x0F;
    break;
  case APU_REG_NOISE_PERIOD:
    apu->noise.mode = (value >> 7) & 0x01;
    apu->noise.timer_reload = NOISE_PERIODS[value & 0x0F];
    break;
  case APU_REG_NOISE_LENGTH:
    apu->noise.length_counter = (value >> 3) & 0x1F;
    apu->noise.envelope_divider = apu->noise.volume;
    apu->noise.envelope_counter = 15;
    break;

  // DMC (simplified)
  case APU_REG_DMC_CTRL:
    apu->dmc.irq_enable = (value >> 7) & 0x01;
    apu->dmc.loop = (value >> 6) & 0x01;
    apu->dmc.rate = value & 0x0F;
    break;
  case APU_REG_DMC_DIRECT:
    apu->dmc.output_level = value & 0x7F;
    break;
  case APU_REG_DMC_ADDR:
    apu->dmc.sample_address = 0xC000 | (value << 6);
    break;
  case APU_REG_DMC_LENGTH:
    apu->dmc.sample_length = (value << 4) | 1;
    break;

  // Status
  case APU_REG_STATUS:
    apu->pulse1_enable = value & 0x01;
    apu->pulse2_enable = value & 0x02;
    apu->triangle_enable = value & 0x04;
    apu->noise_enable = value & 0x08;
    apu->dmc_enable = value & 0x10;
    if (!apu->pulse1_enable)
      apu->pulse1.length_counter = 0;
    if (!apu->pulse2_enable)
      apu->pulse2.length_counter = 0;
    if (!apu->triangle_enable)
      apu->triangle.length_counter = 0;
    if (!apu->noise_enable)
      apu->noise.length_counter = 0;
    break;

  // Frame counter
  case APU_REG_FRAME_COUNTER:
    apu->frame_counter_mode = (value >> 7) & 0x01;
    apu->frame_interrupt = (value >> 6) & 0x01;
    // TODO: Reset frame counter
    break;
  default:
    nes_log_error("ERROR: Invalid APU register write: 0x%04X\n", address);
    break;
  }
}

// Read from an APU register
uint8_t nes_apu_read(APU *apu, uint16_t address)
{
  nes_log_traceback("INFO: APU Reading address: 0x%04X", address);
  if (address == APU_REG_STATUS)
  {
    uint8_t status = 0;
    if (apu->pulse1.length_counter > 0)
      status |= 0x01;
    if (apu->pulse2.length_counter > 0)
      status |= 0x02;
    if (apu->triangle.length_counter > 0)
      status |= 0x04;
    if (apu->noise.length_counter > 0)
      status |= 0x08;
    // TODO: DMC status
    status |= apu->frame_interrupt ? 0x40 : 0x00;
    apu->frame_interrupt = false;
    return status;
  }
  nes_log_error("ERROR: Invalid APU register read: 0x%04X\n", address);
  return 0; // Most APU registers are write-only
}

// Generate audio samples
void nes_apu_generate_samples(APU *apu, size_t sample_count)
{
  for (size_t i = 0; i < sample_count; i++)
  {
    float pulse1_out = 0.0f;
    float pulse2_out = 0.0f;
    float triangle_out = 0.0f;
    float noise_out = 0.0f;
    float dmc_out = 0.0f;

    // Pulse channel 1
    if (apu->pulse1.length_counter > 0 && apu->pulse1.timer_reload >= 8)
    {
      uint8_t duty_step = (uint8_t)(apu->pulse1.phase * 8.0);
      float duty_value = DUTY_CYCLES[apu->pulse1.duty_cycle][duty_step];

      float volume = apu->pulse1.constant_volume ? apu->pulse1.volume / 15.0f : apu->pulse1.envelope_counter / 15.0f;

      pulse1_out = duty_value * volume;

      // Update phase
      double freq = (1789773.0 / (16.0 * (apu->pulse1.timer_reload + 1)));
      apu->pulse1.phase += freq / SAMPLE_RATE;
      if (apu->pulse1.phase >= 1.0)
        apu->pulse1.phase -= 1.0;
    }

    // Pulse channel 2 (similar to pulse1)
    if (apu->pulse2.length_counter > 0 && apu->pulse2.timer_reload >= 8)
    {
      uint8_t duty_step = (uint8_t)(apu->pulse2.phase * 8.0);
      float duty_value = DUTY_CYCLES[apu->pulse2.duty_cycle][duty_step];

      float volume = apu->pulse2.constant_volume ? apu->pulse2.volume / 15.0f : apu->pulse2.envelope_counter / 15.0f;

      pulse2_out = duty_value * volume;

      // Update phase
      double freq = (1789773.0 / (16.0 * (apu->pulse2.timer_reload + 1)));
      apu->pulse2.phase += freq / SAMPLE_RATE;
      if (apu->pulse2.phase >= 1.0)
        apu->pulse2.phase -= 1.0;
    }

    // Triangle channel
    if (apu->triangle.length_counter > 0 && apu->triangle.linear_counter > 0 && apu->triangle.timer_reload >= 2)
    {
      uint8_t tri_step = (uint8_t)(apu->triangle.phase * 32.0);
      triangle_out = TRIANGLE_SEQUENCE[tri_step] / 15.0f;

      // Update phase
      double freq = (1789773.0 / (32.0 * (apu->triangle.timer_reload + 1)));
      apu->triangle.phase += freq / SAMPLE_RATE;
      if (apu->triangle.phase >= 1.0)
        apu->triangle.phase -= 1.0;
    }

    // Noise channel
    if (apu->noise.length_counter > 0)
    {
      if (--apu->noise.timer == 0)
      {
        apu->noise.timer = apu->noise.timer_reload;

        uint16_t feedback;
        if (apu->noise.mode)
        {
          feedback = (apu->noise.shift_register >> 6) & 1;
        }
        else
        {
          feedback = (apu->noise.shift_register >> 1) & 1;
        }
        feedback ^= (apu->noise.shift_register & 1);

        apu->noise.shift_register >>= 1;
        apu->noise.shift_register |= feedback << 14;
      }

      float volume = apu->noise.constant_volume ? apu->noise.volume / 15.0f : apu->noise.envelope_counter / 15.0f;

      noise_out = (apu->noise.shift_register & 1) ? volume : 0.0f;
    }

    // DMC channel (simplified)
    if (apu->dmc_enable)
    {
      dmc_out = apu->dmc.output_level / 128.0f;
    }

    // Mix channels (approximate NES mixing)
    float output = 0.0f;
    output += 0.00752f * pulse1_out;
    output += 0.00752f * pulse2_out;
    output += 0.00851f * triangle_out;
    output += 0.00494f * noise_out;
    output += 0.00335f * dmc_out;

    // Clamp output
    if (output > 1.0f)
      output = 1.0f;
    if (output < -1.0f)
      output = -1.0f;

    apu->audio_buffer[apu->audio_buffer_pos++] = output;

    // Send buffer to audio device when full
    if (apu->audio_buffer_pos >= BUFFER_SIZE)
    {
      // SDL_QueueAudio(apu->audio_device, apu->audio_buffer, BUFFER_SIZE * sizeof(float));
      apu->audio_buffer_pos = 0;
    }
  }
}

// Clock the APU (called at CPU rate)
void nes_apu_clock(APU *apu)
{
  // TODO: Implement frame counter and envelope/linear/length counters

  // For now, just generate samples at the appropriate rate
  static int clock_counter = 0;
  int samples_per_clock = SAMPLE_RATE / 1789773; // ~0.0246 samples per CPU clock

  clock_counter += samples_per_clock;
  if (clock_counter >= 1)
  {
    nes_apu_generate_samples(apu, 1);
    clock_counter -= 1;
  }
}

// Clean up the APU
void nes_apu_cleanup(APU *apu)
{
  // SDL_CloseAudioDevice(apu->audio_device);
}