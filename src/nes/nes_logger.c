#include "nes_logger.h"

void nes_log(const char *format, ...)
{
  if (!log_nes_enabled)
    return;

  FILE *log_file = fopen("log/nes_log.log", "a");
  if (!log_file)
    return;

  va_list args;
  va_start(args, format);
  vfprintf(log_file, format, args);
  va_end(args);

  fclose(log_file);
}

void nes_log_clear()
{
  FILE *log_file = fopen("log/nes_log.log", "w");
  if (!log_file)
    return;

  fclose(log_file);
}
