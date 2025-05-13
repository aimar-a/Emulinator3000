#include "server.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

// Definiciones específicas de plataforma
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#define close_socket closesocket
#define sleep_ms(x) Sleep(x)
typedef SOCKET socket_t;
#define INVALID_SOCKET_VALUE INVALID_SOCKET
#define SOCKET_ERROR_VALUE SOCKET_ERROR
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <dirent.h>
#include <sys/types.h>
#define close_socket close
#define sleep_ms(x) usleep(x * 1000)
typedef int socket_t;
#define INVALID_SOCKET_VALUE -1
#define SOCKET_ERROR_VALUE -1
#define WSAGetLastError() errno
#endif

#include "network_util.h"

// Prototipos de funciones
void loadRomsFromDirectory(const char *dirPath, char romOptions[][128], int *romCount);
void servirChip8();
void servirNES();

// Definir server socket globalmente
socket_t server_socket;
size_t bytes_received;
char buffer[1024];

void server_run()
{
#ifdef _WIN32
  // Inicializar Winsock solo en Windows
  WSADATA wsa;
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
  {
    printf("Error al inicializar Winsock: %d\n", WSAGetLastError());
    return;
  }
#endif

  socket_t client_socket;
  struct sockaddr_in server, client;
#ifdef _WIN32
  int c;
#else
  socklen_t c;
#endif
  uint8_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT / 8]; // Buffer CHIP8 (1 bit por pixel)

  // Crear socket
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET_VALUE)
  {
    printf("Error al crear socket: %d\n", WSAGetLastError());
#ifdef _WIN32
    WSACleanup();
#endif
    return;
  }

  // Configurar dirección del servidor
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(SERVER_PORT);

  // Enlazar socket
  if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR_VALUE)
  {
    printf("Error en bind: %d\n", WSAGetLastError());
    close_socket(server_socket);
#ifdef _WIN32
    WSACleanup();
#endif
    return;
  }

  printf("Servidor iniciado. Esperando conexiones...\n");
  listen(server_socket, 1);

  // Aceptar conexión entrante
  c = sizeof(struct sockaddr_in);
  client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
  if (client_socket == INVALID_SOCKET_VALUE)
  {
    printf("Error al aceptar conexión: %d\n", WSAGetLastError());
    close_socket(server_socket);
#ifdef _WIN32
    WSACleanup();
#endif
    return;
  }

  printf("Cliente conectado\n");

  clienteAnonimo(client_socket);

  // Limpieza
  close_socket(client_socket);
  close_socket(server_socket);
#ifdef _WIN32
  WSACleanup();
#endif
}

void clienteAnonimo(socket_t client_socket)
{
  // Autenticación del cliente
  while (1)
  {
    char is_register;
    char username[32];
    char password[32];

    // Recibir datos de autenticación
    if (!receiveData(client_socket, &is_register, sizeof(is_register), &bytes_received))
    {
      printf("Error al recibir datos de autenticación: %d\n", WSAGetLastError());
      close_socket(client_socket);
      close_socket(server_socket);
      {
        printf("Error al recibir datos de autenticación: %d\n", WSAGetLastError());
        close_socket(client_socket);
        close_socket(server_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return;
      }
    }
    printf("Registro o login: %s\n", is_register ? "Registro" : "Login");

    if (!receiveData(client_socket, username, sizeof(username), &bytes_received))
    {
      printf("Error al recibir nombre de usuario: %d\n", WSAGetLastError());
      close_socket(client_socket);
      close_socket(server_socket);
#ifdef _WIN32
      WSACleanup();
#endif
      return;
    }
    printf("Nombre de usuario: %s\n", username);

    if (!receiveData(client_socket, password, sizeof(password), &bytes_received))
    {
      printf("Error al recibir contraseña: %d\n", WSAGetLastError());
      close_socket(client_socket);
      close_socket(server_socket);
#ifdef _WIN32
      WSACleanup();
#endif
      return;
    }
    printf("Contraseña: %s\n", password);

    // Validar campos vacíos
    if (username[0] == '\0' || password[0] == '\0')
    {
      printf("Nombre de usuario o contraseña vacíos\n");
      if (!sendData(client_socket, "ERR", 3))
      {
        printf("Error al enviar error: %d\n", WSAGetLastError());
        close_socket(client_socket);
        close_socket(server_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return;
      }
      continue;
    }

    // Registrar o autenticar usuario
    if (is_register)
    {
      if (!existeUsuario(username))
      {
        insertarUsuarios(username, password);
        printf("Usuario registrado: %s\n", username);
        if (!sendData(client_socket, "ACK", 3))
        {
          printf("Error al enviar confirmación: %d\n", WSAGetLastError());
          close_socket(client_socket);
          close_socket(server_socket);
#ifdef _WIN32
          WSACleanup();
#endif
          return;
        }
        clienteConocido(client_socket, username);
      }
      else
      {
        printf("Usuario ya existe: %s\n", username);
        if (sendData(client_socket, "ERR", 3))
        {
          printf("Error al enviar error: %d\n", WSAGetLastError());
          close_socket(client_socket);
          close_socket(server_socket);
#ifdef _WIN32
          WSACleanup();
#endif
          return;
        }
      }
    }
    else
    {
      if (existeUsuarioYPas(username, password))
      {
        printf("Usuario autenticado: %s\n", username);
        if (!sendData(client_socket, "ACK", 3))
        {
          printf("Error al enviar confirmación: %d\n", WSAGetLastError());
          close_socket(client_socket);
          close_socket(server_socket);
#ifdef _WIN32
          WSACleanup();
#endif
          return;
        }
        clienteConocido(client_socket, username);
      }
      else
      {
        printf("Autenticación fallida: %s\n", username);
        if (!sendData(client_socket, "ERR", 3))
        {
          printf("Error al enviar error: %d\n", WSAGetLastError());
          close_socket(client_socket);
          close_socket(server_socket);
#ifdef _WIN32
          WSACleanup();
#endif
          return;
        }
      }
    }
  }
}

void clienteConocido(socket_t client_socket, char *username)
{ // Bucle principal del servidor
  while (1)
  {
    uint8_t selection;
    if (!receiveData(client_socket, &selection, sizeof(selection), &bytes_received))
    {
      printf("Error al recibir opción: %d\n", WSAGetLastError());
      close_socket(client_socket);
      close_socket(server_socket);
#ifdef _WIN32
      WSACleanup();
#endif
      return;
    }

    printf("Opción seleccionada: %02X\n", selection);

    switch (selection)
    {
    case 0xE0: // Emular CHIP8
    {
      printf("Emulando CHIP8...\n");
      char selectedRom[128];
      if (!receiveData(client_socket, selectedRom, sizeof(selectedRom), &bytes_received))
      {
        printf("Error al recibir ROM: %d\n", WSAGetLastError());
        close_socket(client_socket);
        close_socket(server_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return;
      }
      printf("ROM seleccionada: %s\n", selectedRom);
      servirChip8();
      break;
    }

    case 0xE1: // Emular NES
    {
      printf("Emulando NES...\n");
      char selectedRom[128];
      if (!receiveData(client_socket, selectedRom, sizeof(selectedRom), &bytes_received))
      {
        printf("Error al recibir ROM: %d\n", WSAGetLastError());
        close_socket(client_socket);
        close_socket(server_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return;
      }
      printf("ROM seleccionada: %s\n", selectedRom);
      servirNES();
      break;
    }

    case 0x01: // Cambiar contraseña
      printf("Cambiando contraseña...\n");
      // Implementar cambio de contraseña
      break;

    case 0x02: // Enviar ROMs CHIP8
    {
      printf("Enviando ROMs CHIP8...\n");
      char romOptions[128][128];
      int romCount = 0;
      loadRomsFromDirectory("resources/chip8-roms/games", romOptions, &romCount);

      if (!sendData(client_socket, (char *)&romCount, sizeof(romCount)))
      {
        printf("Error al enviar conteo ROMs: %d\n", WSAGetLastError());
        close_socket(client_socket);
        close_socket(server_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return;
      }

      for (int i = 0; i < romCount; i++)
      {
        if (!sendData(client_socket, romOptions[i], sizeof(romOptions[i])))
        {
          printf("Error al enviar ROM: %d\n", WSAGetLastError());
          close_socket(client_socket);
          close_socket(server_socket);
#ifdef _WIN32
          WSACleanup();
#endif
          return;
        }
      }
      break;
    }

    case 0x03: // Enviar ROMs NES
      printf("Enviando ROMs NES...\n");
      // Implementar similar a CHIP8
      break;

    default:
      printf("Opción no válida\n");
      close_socket(client_socket);
      close_socket(server_socket);
#ifdef _WIN32
      WSACleanup();
#endif
      return;
    }
  }
}

void loadRomsFromDirectory(const char *dirPath, char romOptions[][128], int *romCount)
{
#ifdef _WIN32
  WIN32_FIND_DATA findFileData;
  HANDLE hFind = FindFirstFile((dirPath + "\\*.ch8"), &findFileData);

  *romCount = 0;

  if (hFind != INVALID_HANDLE_VALUE)
  {
    do
    {
      if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
        strcpy(romOptions[*romCount], findFileData.cFileName);
        (*romCount)++;
      }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
  }
#else
  DIR *dir = opendir(dirPath);
  if (dir != NULL)
  {
    struct dirent *entry;
    *romCount = 0;

    while ((entry = readdir(dir)) != NULL && *romCount < 128)
    {
      if (strstr(entry->d_name, ".ch8") != NULL)
      {
        strcpy(romOptions[*romCount], entry->d_name);
        (*romCount)++;
      }
    }
    closedir(dir);
  }
#endif
}

void servirChip8()
{
  while (1)
  {
    // Lógica de emulación CHIP8
    sleep_ms(16); // ~60 FPS

    // Verificar si el cliente sigue conectado
    // Podrías implementar un heartbeat aquí
  }
}

void servirNES()
{
  while (1)
  {
    // Lógica de emulación NES
    sleep_ms(16); // ~60 FPS
  }
}