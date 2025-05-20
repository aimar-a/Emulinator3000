#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#define CHIP8_ROMS_PATH "resources/chip8-roms"
#define NES_ROMS_PATH "resources/nes-roms"
#define MAX_ROM_NAME_LENGTH 128
#define MAX_ROM_PATH_LENGTH 256

void listarROMsRecursivo(const char *directory, char roms[][MAX_ROM_PATH_LENGTH], int *count);
void extraerNombreROM(const char *entrada, char *nombre);
void listarROMsRecursivoNES(const char *directory, char roms[][MAX_ROM_PATH_LENGTH], int *count);

#endif