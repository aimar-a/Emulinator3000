#include "nes_logger.h"

const int nes_traceback_size = TRACEBACK_SIZE;
char nes_traceback[TRACEBACK_SIZE][TRACEBACK_LINE_SIZE];
int nes_traceback_count = 0;

bool nes_terminate = false;

void nes_log_instant(const char *format, ...)
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

void nes_log_traceback(const char *format, ...)
{
  if (format[0] == 'E')
  {
    nes_log_error("ERROR: This is an error message\n%s", format);
    return;
  }
  else if (format[0] == 'W')
  {
    nes_log_error("ERROR: This is a warning message\n%s", format);
    return;
  }

  if (!log_nes_enabled)
    return;

  if (nes_traceback_count >= nes_traceback_size)
    nes_traceback_count = 0;

  va_list args;
  va_start(args, format);
  vsnprintf(nes_traceback[nes_traceback_count], nes_traceback_size, format, args);
  va_end(args);

  nes_traceback_count++;
}

void nes_log_error(const char *format, ...)
{
  if (format[0] != 'E')
  {
    nes_log_error("ERROR: This is not an error message\n%s", format);
    return;
  }

  nes_log_instant("\nERROR DETECTED... TERMINATING PROGRAM\n\n");

  FILE *log_file = fopen("log/nes_log.log", "a");
  if (!log_file)
    return;

  va_list args;
  va_start(args, format);
  vfprintf(log_file, format, args);
  va_end(args);

  fclose(log_file);

  printf("\nERROR DETECTED... TERMINATING PROGRAM\n\n");

  // Stop the program
  nes_terminate = true;
}

void nes_save_traceback()
{
  if (!log_nes_enabled)
    return;

  FILE *log_file = fopen("log/nes_log.log", "a");
  if (!log_file)
    return;

  for (int i = nes_traceback_count + 1; i != nes_traceback_count; i++)
  {
    if (i >= nes_traceback_size)
      i = 0;

    fprintf(log_file, "%s", nes_traceback[i]);
  }

  fclose(log_file);
}

void nes_log_clear()
{
  FILE *log_file = fopen("log/nes_log.log", "w");
  if (!log_file)
    return;

  fclose(log_file);
}
