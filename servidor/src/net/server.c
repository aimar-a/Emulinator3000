#include "server.h"

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

  while (1)
  {
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
  }

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
    char option;
    char username[32];
    char password[32];

    // Recibir datos de autenticación
    if (!receiveData(client_socket, &option, sizeof(option), &bytes_received))
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
    printf("Opción seleccionada: %02X\n", option);

    if (option == 0x00) // Salir
    {
      printf("Cliente se desconecta...\n");
      close_socket(client_socket);
      return;
    }

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
      if (!sendData(client_socket, "ERR", 4))
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

    if (option == 0x02) // Registro
    {
      if (!existeUsuario(username))
      {
        insertarUsuarios(username, password);
        printf("Usuario registrado: %s\n", username);
        if (!sendData(client_socket, "ACK", 4))
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
        if (!sendData(client_socket, "ERR", 3))
        {
          printf("Error al enviar error1: %d\n", WSAGetLastError());
          close_socket(client_socket);
          close_socket(server_socket);
#ifdef _WIN32
          WSACleanup();
#endif
          return;
        }
      }
    }
    else if (option == 0x01) // Autenticación
    {
      if (existeUsuarioYPas(username, password))
      {
        printf("Usuario autenticado: %s\n", username);
        if (!sendData(client_socket, "ACK", 4))
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
        if (!sendData(client_socket, "ERR", 4))
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
      servirChip8(client_socket, selectedRom);
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

    case 0x00:
      printf("Cliente cierra sesion...\n");
      return;

    case 0x01: // Cambiar contraseña
      printf("Cambiando contraseña...\n");
      // Recibir vieja contraseña
      if (!receiveData(client_socket, buffer, sizeof(buffer), &bytes_received))
      {
        printf("Error al recibir contraseña: %d\n", WSAGetLastError());
        close_socket(client_socket);
        close_socket(server_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return;
      }
      printf("Contraseña vieja: %s\n", buffer);
      if (!existeUsuarioYPas(username, buffer))
      {
        printf("Contraseña incorrecta\n");
        if (!sendData(client_socket, "ERR", 4))
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
      else
      {
        printf("Contraseña correcta\n");
        if (!sendData(client_socket, "ACK", 4))
        {
          printf("Error al enviar confirmación: %d\n", WSAGetLastError());
          close_socket(client_socket);
          close_socket(server_socket);
#ifdef _WIN32
          WSACleanup();
#endif
          return;
        }
      }
      // Recibir nueva contraseña
      if (!receiveData(client_socket, buffer, sizeof(buffer), &bytes_received))
      {
        printf("Error al recibir nueva contraseña: %d\n", WSAGetLastError());
        close_socket(client_socket);
        close_socket(server_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return;
      }
      printf("Nueva contraseña: %s\n", buffer);
      if (!updateContrasena(buffer, username))
      {
        printf("Error al cambiar contraseña\n");
        if (!sendData(client_socket, "ERR", 4))
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
      else
      {
        printf("Contraseña cambiada con éxito\n");
        if (!sendData(client_socket, "ACK", 4))
        {
          printf("Error al enviar confirmación: %d\n", WSAGetLastError());
          close_socket(client_socket);
          close_socket(server_socket);
#ifdef _WIN32
          WSACleanup();
#endif
          return;
        }
      }
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
    case 0x70: // Perfil
    {
        while (1) {

            uint8_t selection2;
            if (!receiveData(client_socket, &selection2, sizeof(selection2), &bytes_received)) {
                printf("Error al recibir VER PERFIL: %d\n", WSAGetLastError());
                close_socket(client_socket);
                close_socket(server_socket);
                #ifdef _WIN32
                WSACleanup();
                #endif
                return;
            }

            switch (selection2) {
                case 0x11:
                    printf("Ver Logros..\n");
                    break;

                case 0x12: 
                  printf("Ver Amigos..\n");
                  break; 
                case 0x13:
                    printf("Ver Tiempo Jugado...\n");

                    char user[256];  // Reservamos espacio para el nombre de usuario
                    char **nombreJuegos = NULL;
                    int *tiemposSegundos = NULL;
                    int cantidadJuegos = 0;

                    // Recibimos el nombre del usuario
                    if (!receiveData(client_socket, user, sizeof(user), &bytes_received)) {
                        printf("Error al recibir nombre de usuario: %d\n", WSAGetLastError());
                        close_socket(client_socket);
                        close_socket(server_socket);
                #ifdef _WIN32
                        WSACleanup();
                #endif
                        return;
                    }

                    printf("Nombre de usuario: %s\n", user);  // Usar `user`, no `username`

                    // Obtenemos la cantidad de juegos y datos asociados
                    cantidadJuegos = getTiempoJugadoTodosLosJuegos(user, &nombreJuegos, &tiemposSegundos);

                    // Enviamos cantidad de juegos al cliente
                    if (!sendData(client_socket, &cantidadJuegos, sizeof(cantidadJuegos))) {
                        printf("Error al enviar CantidadJuegos: %d\n", WSAGetLastError());
                        close_socket(client_socket);
                        close_socket(server_socket);
                #ifdef _WIN32
                        WSACleanup();
                #endif
                        return;
                    }

                    // Aquí podrías enviar también los nombres de juegos y los tiempos si es necesario

                    break;
              
    
                case 0x10: 
                  printf("Volviendo..\n");
                  break;
            }
        }
        break;
    }



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
  char searchPath[MAX_PATH];
  sprintf(searchPath, "%s\\*.ch8", dirPath);
  HANDLE hFind = FindFirstFile(searchPath, &findFileData);

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

void servirChip8(socket_t sock, char *selectedRom)
{
  char fullRomPath[256];
  sprintf(fullRomPath, "resources/chip8-roms/games/%s", selectedRom);
  Chip8 *chip8 = chip8init(fullRomPath);
  while (1)
  {
    // Lógica de emulación CHIP8
    chip8step(chip8);

    // Enviar datos de pantalla al cliente
    if (!sendData(sock, chip8->pantalla, SCREEN_WIDTH * SCREEN_HEIGHT / 8))
    {
      printf("Error al enviar datos de pantalla: %d\n", WSAGetLastError());
      break;
    }

    // Lógica de emulación CHIP8
    sleep_ms(16); // ~60 FPS

    // Verificar si el cliente sigue conectado
    // Podrías implementar un heartbeat aquí
    if (chip8->esc)
    {
      printf("Cliente solicita salir...\n");
      break;
    }
  }
  chip8terminate(chip8);
}

void servirNES()
{
  while (1)
  {
    // Lógica de emulación NES
    sleep_ms(16); // ~60 FPS
  }
}