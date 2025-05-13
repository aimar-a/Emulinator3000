#include "server.h"

void server_run()
{
  WSADATA wsa;
  SOCKET server_socket, client_socket;
  struct sockaddr_in server, client;
  int c, bytes_sent;
  uint8_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT / 8]; // Buffer CHIP8 (1 bit por pixel)

  // Inicializar Winsock
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
  {
    printf("Error al inicializar Winsock: %d\n", WSAGetLastError());
    return;
  }

  // Crear socket
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
  {
    printf("Error al crear socket: %d\n", WSAGetLastError());
    return;
  }

  // Configurar dirección del servidor
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY; // inet_addr(SERVER_IP);
  server.sin_port = htons(SERVER_PORT);

  // Enlazar socket
  if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
  {
    printf("Error en bind: %d\n", WSAGetLastError());
    closesocket(server_socket);
    return;
  }

  printf("Servidor iniciado. Esperando conexiones...\n");
  listen(server_socket, 1);

  // Aceptar conexión entrante
  c = sizeof(struct sockaddr_in);
  client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
  if (client_socket == INVALID_SOCKET)
  {
    printf("Error al aceptar conexión: %d\n", WSAGetLastError());
    closesocket(server_socket);
    return;
  }

  printf("Cliente conectado\n");
  // primero estaria bien autentificar al cliente
  while (true)
  {
    // 1. Recibir el nombre de usuario y la contraseña y si quiere registrarse o login
    uint8_t is_register;
    char username[32];
    char password[32];
    bytes_sent = recv(client_socket, &is_register, sizeof(is_register), 0);
    if (bytes_sent == SOCKET_ERROR)
    {
      printf("Error al recibir registro: %d\n", WSAGetLastError());
      closesocket(client_socket);
      closesocket(server_socket);
      WSACleanup();
      return;
    }
    bytes_sent = recv(client_socket, username, sizeof(username), 0);
    if (bytes_sent == SOCKET_ERROR)
    {
      printf("Error al recibir nombre de usuario: %d\n", WSAGetLastError());
      closesocket(client_socket);
      closesocket(server_socket);
      WSACleanup();
      return;
    }
    bytes_sent = recv(client_socket, password, sizeof(password), 0);
    if (bytes_sent == SOCKET_ERROR)
    {
      printf("Error al recibir contraseña: %d\n", WSAGetLastError());
      closesocket(client_socket);
      closesocket(server_socket);
      WSACleanup();
      return;
    }

    // Asegurarse de que el nombre de usuario y la contraseña no estén vacíos
    if (username[0] == '\0' || password[0] == '\0')
    {
      printf("Nombre de usuario o contraseña vacíos\n");
      bytes_sent = send(client_socket, "ERROR", 5, 0);
      if (bytes_sent == SOCKET_ERROR)
      {
        printf("Error al enviar error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return;
      }
      continue; // Volver a solicitar el nombre de usuario y la contraseña
    }

    // Registrar usuario
    if (is_register)
    {
      if (existeUsuario(username) != true)
      {
        insertarUsuarios(username, password);
        printf("Usuario registrado correctamente\n");
        printf("Usuario: %s\n", username);
        printf("Contrasenya: %s\n", password);
        bytes_sent = send(client_socket, "OK", 2, 0);
        if (bytes_sent == SOCKET_ERROR)
        {
          printf("Error al enviar confirmación: %d\n", WSAGetLastError());
          closesocket(client_socket);
          closesocket(server_socket);
          WSACleanup();
          return;
        }
        break; // Salir del bucle de autenticación
      }
      else
      {
        printf("El usuario ya existe\n");
        printf("Usuario: %s\n", username);
        printf("Contrasenya: %s\n", password);
        bytes_sent = send(client_socket, "ERROR", 5, 0);
        if (bytes_sent == SOCKET_ERROR)
        {
          printf("Error al enviar error: %d\n", WSAGetLastError());
          closesocket(client_socket);
          closesocket(server_socket);
          WSACleanup();
          return;
        }
      }
    }
    // Login
    else
    {
      if (existeUsuarioYPas(username, password) == true)
      {
        printf("Usuario y contrasenya correctos\n");
        printf("Usuario: %s\n", username);
        printf("Contrasenya: %s\n", password);
        bytes_sent = send(client_socket, "OK", 2, 0);
        if (bytes_sent == SOCKET_ERROR)
        {
          printf("Error al enviar confirmación: %d\n", WSAGetLastError());
          closesocket(client_socket);
          closesocket(server_socket);
          WSACleanup();
          return;
        }
        break; // Salir del bucle de autenticación
      }
      else
      {
        printf("Usuario o contrasenya incorrectos\n");
        printf("Usuario: %s\n", username);
        printf("Contrasenya: %s\n", password);
        bytes_sent = send(client_socket, "ERROR", 5, 0);

        if (bytes_sent == SOCKET_ERROR)
        {
          printf("Error al enviar error: %d\n", WSAGetLastError());
          closesocket(client_socket);
          closesocket(server_socket);
          WSACleanup();
          return;
        }
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return;
      }
    }
  }

  while (true)
  {
    uint8_t selection;
    bytes_sent = recv(client_socket, &selection, sizeof(selection), 0);
    if (bytes_sent == SOCKET_ERROR)
    {
      printf("Error al recibir selección: %d\n", WSAGetLastError());
      closesocket(client_socket);
      closesocket(server_socket);
      WSACleanup();
      return;
    }
    printf("Seleccionado: %02X\n", selection);
    /*
      0xE0 -> Emular Chip8
      0xE1 -> Emular NES

      0x00 -> Salir
      0x01 -> Cambiar contraseña
      0x02 -> Enviar roms de Chip8
      0x03 -> Enviar roms de NES
      ... mas opciones de perfilUsuario a implementar TODO
    */

    if (selection == 0xE0)
    {
      // Emular CHIP8
      printf("Emulando CHIP8...\n");
      // Aquí iría el código de emulación de CHIP8
      // 1. Obtener la ROM seleccionada
      char selectedRom[128];
      bytes_sent = recv(client_socket, selectedRom, sizeof(selectedRom), 0);
      if (bytes_sent == SOCKET_ERROR)
      {
        printf("Error al recibir ROM seleccionada: %d\n", WSAGetLastError());
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return;
      }
      printf("ROM seleccionada: %s\n", selectedRom);
      // 2. Ejecutar la ROM
      char romPath[256];
      sprintf(romPath, "resources/chip8-roms/games/%s", selectedRom);
      // chip8cpuLaunch(romPath);

      servirChip8();
    }
    else if (selection == 0xE1)
    {
      // Emular NES
      printf("Emulando NES...\n");
      // Aquí iría el código de emulación de NES
      // 1. Obtener la ROM seleccionada
      char selectedRom[128];
      bytes_sent = recv(client_socket, selectedRom, sizeof(selectedRom), 0);
      if (bytes_sent == SOCKET_ERROR)
      {
        printf("Error al recibir ROM seleccionada: %d\n", WSAGetLastError());
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return;
      }
      printf("ROM seleccionada: %s\n", selectedRom);
      // 2. Ejecutar la ROM
      char romPath[256];
      sprintf(romPath, "resources/nes-roms/games/%s", selectedRom);
      // nes_launch(romPath);

      servirNES();
    }
    else if (selection == 0x01)
    {
      // Cambiar contraseña
      printf("Cambiando contraseña...\n");
      // Aquí iría el código para cambiar la contraseña
    }
    else if (selection == 0x02)
    {
      // Enviar roms de CHIP8
      printf("Enviando roms de CHIP8...\n");
      // Aquí iría el código para enviar las roms de CHIP8
      char romOptions[128][128];
      int romCount = 0;
      loadRomsFromDirectory("resources/chip8-roms/games", romOptions, &romCount);
      // Enviar la lista de ROMs al cliente
      bytes_sent = send(client_socket, (char *)&romCount, sizeof(romCount), 0);
      if (bytes_sent == SOCKET_ERROR)
      {
        printf("Error al enviar lista de ROMs: %d\n", WSAGetLastError());
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return;
      }
      for (int i = 0; i < romCount; i++)
      {
        bytes_sent = send(client_socket, romOptions[i], sizeof(romOptions[i]), 0);
        if (bytes_sent == SOCKET_ERROR)
        {
          printf("Error al enviar ROM: %d\n", WSAGetLastError());
          closesocket(client_socket);
          closesocket(server_socket);
          WSACleanup();
          return;
        }
      }
      printf("Roms de CHIP8 enviadas\n");
    }
    else if (selection == 0x03)
    {
      // Enviar roms de NES
      printf("Enviando roms de NES...\n");
      // Aquí iría el código para enviar las roms de NES
      // TODO
    }
    else
    {
      printf("Opción no válida\n");
      closesocket(client_socket);
      closesocket(server_socket);
      WSACleanup();
      return;
    }
  }

  // Limpieza
  closesocket(client_socket);
  closesocket(server_socket);
  WSACleanup();
}

void loadRomsFromDirectory(const char *dirPath, char romOptions[][128], int *romCount)
{
  DIR *dir = opendir(dirPath);
  if (dir == NULL)
  {
    printf("No se pudo abrir el directorio %s\n", dirPath);
    return;
  }

  struct dirent *entry;
  *romCount = 0;

  while ((entry = readdir(dir)) != NULL)
  {
    if (strstr(entry->d_name, ".ch8") != NULL)
    {
      strcpy(romOptions[*romCount], entry->d_name);
      (*romCount)++;
    }
  }

  closedir(dir);
}

void servirChip8()
{
  // Aquí iría el código para servir la emulación de CHIP8
  // Por ejemplo, podrías enviar frames al cliente a través del socket
  // y recibir comandos desde el cliente.

  while (1)
  {
    // 1. Aquí ejecutas un ciclo de emulación que actualice el buffer 'pixels'
    //    (tu código existente de CHIP8/NES)

    // 2. Enviar frame al cliente
    uint16_t width = htons(SCREEN_WIDTH);
    uint16_t height = htons(SCREEN_HEIGHT);

    // Enviar dimensiones
    // bytes_sent = send(client_socket, (char *)&width, sizeof(width), 0);
    // bytes_sent = send(client_socket, (char *)&height, sizeof(height), 0);

    // Enviar datos de píxeles
    // bytes_sent = send(client_socket, (char *)pixels, sizeof(pixels), 0);

    // if (bytes_sent == SOCKET_ERROR)
    //{
    //   printf("Error al enviar datos: %d\n", WSAGetLastError());
    //   break;
    // }

    // Pequeña pausa para controlar el FPS
    Sleep(16); // ~60 FPS
  }
}

void servirNES()
{
  // Aquí iría el código para servir la emulación de NES
  // Por ejemplo, podrías enviar frames al cliente a través del socket
  // y recibir comandos desde el cliente.
}