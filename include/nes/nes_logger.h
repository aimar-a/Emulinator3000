#ifndef NES_LOGGER_H
#define NES_LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include "config_file.h"

#define TRACEBACK_SIZE 200      // Tamaño del traceback
#define TRACEBACK_LINE_SIZE 256 // Tamaño de cada línea del traceback

extern char nes_traceback[][TRACEBACK_LINE_SIZE];
extern const int nes_traceback_size;
extern int nes_traceback_count;

extern bool nes_terminate;

// Escribe en el logger al instante
void nes_log_instant(const char *format, ...);
// Guarda el log en una variable para el traceback
void nes_log_traceback(const char *format, ...);
// Para el programa y se escribe el traceback + el error
void nes_log_error(const char *format, ...);
// Guarda el traceback en el log file
void nes_save_traceback();
// Limpia el log file
void nes_log_clear();

#endif