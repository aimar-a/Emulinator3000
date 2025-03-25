#include "nes_logger.h"

void nes_log(const char *format, ...)
{
  FILE *log_file = fopen("log/nes_log.txt", "a");
  if (!log_file)
    return;

  va_list args;
  va_start(args, format);
  vfprintf(log_file, format, args);
  va_end(args);

  fclose(log_file);
}
