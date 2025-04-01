#ifndef CHIP8_LOGGER_H
#define CHIP8_LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include "config_file.h"

void chip8_log(const char *format, ...);
void chip8_log_clear();

#endif