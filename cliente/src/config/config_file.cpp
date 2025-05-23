#include "config_file.hpp"
#include <fstream>
#include <vector>
#include <algorithm>

int SCREEN_WIDTH_CHIP8 = 64;
int SCREEN_HEIGHT_CHIP8 = 32;
int SCREEN_SCALE_CHIP8 = 26;

int SCREEN_WIDTH_SUPERCHIP = 128;
int SCREEN_HEIGHT_SUPERCHIP = 64;
int SCREEN_SCALE_SUPERCHIP = 13;

int SCREEN_WIDTH_NES = 256;
int SCREEN_HEIGHT_NES = 240;
int SCREEN_SCALE_NES = 4;

const std::string CONFIG_FILE = "resources/config/client.conf";

// Función para cargar la configuración desde el archivo .txt
void cargarConfiguracion()
{
  std::ifstream archivo(CONFIG_FILE);
  if (!archivo.is_open())
  {
    printf("ERROR: No se pudo abrir el archivo de configuración: %s\n", CONFIG_FILE.c_str());
    return;
  }

  std::string linea;
  while (std::getline(archivo, linea))
  {
    // Eliminar espacios en blanco al inicio y final
    linea.erase(0, linea.find_first_not_of(" \t"));
    linea.erase(linea.find_last_not_of(" \t") + 1);

    // Ignorar líneas vacías o comentarios
    if (linea.empty() || linea[0] == '#')
    {
      continue;
    }

    // Leer configuraciones para Chip8
    if (linea.find("SCREEN_WIDTH_CHIP") != std::string::npos)
    {
      sscanf(linea.c_str(), "SCREEN_WIDTH_CHIP=%d", &SCREEN_WIDTH_CHIP8);
    }
    else if (linea.find("SCREEN_HEIGHT_CHIP") != std::string::npos)
    {
      sscanf(linea.c_str(), "SCREEN_HEIGHT_CHIP=%d", &SCREEN_HEIGHT_CHIP8);
    }
    else if (linea.find("SCREEN_SCALE_CHIP") != std::string::npos)
    {
      sscanf(linea.c_str(), "SCREEN_SCALE_CHIP=%d", &SCREEN_SCALE_CHIP8);
    }

    // Leer configuraciones para SuperChip8
    else if (linea.find("SCREEN_WIDTH_SUPERCHIP") != std::string::npos)
    {
      sscanf(linea.c_str(), "SCREEN_WIDTH_SUPERCHIP=%d", &SCREEN_WIDTH_SUPERCHIP);
    }
    else if (linea.find("SCREEN_HEIGHT_SUPERCHIP") != std::string::npos)
    {
      sscanf(linea.c_str(), "SCREEN_HEIGHT_SUPERCHIP=%d", &SCREEN_HEIGHT_SUPERCHIP);
    }
    else if (linea.find("SCREEN_SCALE_SUPERCHIP") != std::string::npos)
    {
      sscanf(linea.c_str(), "SCREEN_SCALE_SUPERCHIP=%d", &SCREEN_SCALE_SUPERCHIP);
    }

    // Leer configuraciones para NES
    else if (linea.find("SCREEN_WIDTH_NES") != std::string::npos)
    {
      sscanf(linea.c_str(), "SCREEN_WIDTH_NES=%d", &SCREEN_WIDTH_NES);
    }
    else if (linea.find("SCREEN_HEIGHT_NES") != std::string::npos)
    {
      sscanf(linea.c_str(), "SCREEN_HEIGHT_NES=%d", &SCREEN_HEIGHT_NES);
    }
    else if (linea.find("SCREEN_SCALE_NES") != std::string::npos)
    {
      sscanf(linea.c_str(), "SCREEN_SCALE_NES=%d", &SCREEN_SCALE_NES);
    }
  }
}

int cambiarEscala(int &variableEscala, const std::string &configBuscar, int nuevaEscala)
{
  std::ifstream archivoEntrada(CONFIG_FILE);
  if (!archivoEntrada.is_open())
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para leer: %s\n", CONFIG_FILE.c_str());
    return 1;
  }

  std::vector<std::string> lineas;
  std::string linea;
  bool encontrado = false;

  while (std::getline(archivoEntrada, linea))
  {
    if (linea.find(configBuscar) != std::string::npos)
    {
      linea = configBuscar + "=" + std::to_string(nuevaEscala);
      encontrado = true;
    }
    lineas.push_back(linea);
  }
  archivoEntrada.close();

  if (!encontrado)
  {
    printf("ERROR: No se encontró la línea %s en el archivo de configuración.\n", configBuscar.c_str());
    return 1;
  }

  std::ofstream archivoSalida(CONFIG_FILE);
  if (!archivoSalida.is_open())
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para escribir: %s\n", CONFIG_FILE.c_str());
    return 1;
  }

  for (const auto &l : lineas)
  {
    archivoSalida << l << "\n";
  }
  archivoSalida.close();

  variableEscala = nuevaEscala;
  return 0;
}

int cambiarEscalaChip8(int nuevaEscala)
{
  return cambiarEscala(SCREEN_SCALE_CHIP8, "SCREEN_SCALE_CHIP", nuevaEscala);
}

int cambiarEscalaSuperChip(int nuevaEscala)
{
  return cambiarEscala(SCREEN_SCALE_SUPERCHIP, "SCREEN_SCALE_SUPERCHIP", nuevaEscala);
}

int cambiarEscalaNes(int nuevaEscala)
{
  return cambiarEscala(SCREEN_SCALE_NES, "SCREEN_SCALE_NES", nuevaEscala);
}