#ifndef NES_LOGGER_H
#define NES_LOGGER_H

#include <stdio.h>
#include <stdarg.h>

void nes_log(const char *format, ...);
void nes_log_clear();

#endif