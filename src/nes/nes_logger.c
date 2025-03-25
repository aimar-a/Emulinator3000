#include "nes_logger.h"

void nes_log(const char *format, ...)
{
  FILE *log_file = fopen("log/nes_log.log", "a");
  if (!log_file)
    return;

  va_list args;
  va_start(args, format);
  vfprintf(log_file, format, args);
  va_end(args);

  fclose(log_file);

  printf(format, args); // Por ahora también lo imprimimos en consola pa no tener que abrir el archivo
}
