#include "chip8_logger.h"

void chip8_log(const char *format, ...)
{
  if (!log_chip8_enabled)
    return;

  FILE *log_file = fopen("log/chip8_log.log", "a");
  if (!log_file)
    return;

  va_list args;
  va_start(args, format);
  vfprintf(log_file, format, args);
  va_end(args);

  fclose(log_file);
}

void chip8_log_clear()
{
  FILE *log_file = fopen("log/chip8_log.log", "w");
  if (!log_file)
    return;

  fclose(log_file);
}
