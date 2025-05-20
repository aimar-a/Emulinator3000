#include "directories.h"

void listarROMsRecursivo(const char *directory, char roms[][MAX_ROM_PATH_LENGTH], int *count)
{
  struct dirent *entry;
  DIR *dp = opendir(directory);

  if (dp == NULL)
  {
    perror("Error al abrir el directorio");
    return;
  }
  while ((entry = readdir(dp)) != NULL)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

    struct stat path_stat;
    if (stat(path, &path_stat) == 0)
    {
      if (S_ISREG(path_stat.st_mode))
      { // Archivo regular
        const char *ext = strrchr(entry->d_name, '.');
        if (ext != NULL && strcmp(ext, ".ch8") == 0)
        {
          snprintf(roms[*count], MAX_ROM_PATH_LENGTH, "%s", path);
          (*count)++;
        }
      }
      else if (S_ISDIR(path_stat.st_mode))
      { // Directorio
        listarROMsRecursivo(path, roms, count);
      }
    }
  }
  closedir(dp);
}

void extraerNombreROM(const char *entrada, char *nombre)
{
  // Encontrar el primer espacio después del número
  const char *ruta = entrada;

  // Buscar la última aparición de '/'
  const char *ultimoSlash = strrchr(ruta, '/');
  if (ultimoSlash)
  {
    ruta = ultimoSlash + 1; // Tomar solo el nombre del archivo
  }

  // Copiar el nombre con extensión
  strcpy(nombre, ruta);

  // Buscar la última aparición de '.' para quitar la extensión
  char *punto = strrchr(nombre, '.');
  if (punto)
  {
    *punto = '\0'; // Eliminar la extensión
  }
}

// ns si ya existia una funcion para listas los de la nes, porsiacaso la he creado aqui:
void listarROMsRecursivoNES(const char *directory, char roms[][MAX_ROM_PATH_LENGTH], int *count)
{
  struct dirent *entry;
  DIR *dp = opendir(directory);

  if (dp == NULL)
  {
    perror("Error al abrir el directorio");
    return;
  }
  while ((entry = readdir(dp)) != NULL)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

    struct stat path_stat;
    if (stat(path, &path_stat) == 0)
    {
      if (S_ISREG(path_stat.st_mode))
      { // Archivo regular
        const char *ext = strrchr(entry->d_name, '.');
        if (ext != NULL && strcmp(ext, ".nes") == 0)
        {
          snprintf(roms[*count], MAX_ROM_PATH_LENGTH, "%s", path);
          (*count)++;
        }
      }
      else if (S_ISDIR(path_stat.st_mode))
      { // Directorio
        listarROMsRecursivo(path, roms, count);
      }
    }
  }
  closedir(dp);
}
