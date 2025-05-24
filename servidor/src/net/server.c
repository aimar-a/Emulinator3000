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

  struct sockaddr_in server;

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

  // Escuchar conexiones entrantes
  server_listen();
}

void server_listen()
{
  struct sockaddr_in client;
  socket_t client_socket;
  while (1)
  {
    printf("Servidor iniciado. Esperando conexiones...\n");
    listen(server_socket, 1);

#ifdef _WIN32
    int c;
#else
    socklen_t c;
#endif

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

  disconnectClient(client_socket);
}

void disconnectClient(socket_t client_socket)
{
  close_socket(client_socket);
  printf("Cliente desconectado\n");
  server_listen();
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
      disconnectClient(client_socket);
    }
    printf("Opción seleccionada: %02X\n", option);

    if (option == 0x00) // Salir
    {
      printf("Cliente se desconecta...\n");
      disconnectClient(client_socket);
      return;
    }

    if (!receiveData(client_socket, username, sizeof(username), &bytes_received))
    {
      printf("Error al recibir nombre de usuario: %d\n", WSAGetLastError());
      disconnectClient(client_socket);
    }
    printf("Nombre de usuario: %s\n", username);

    if (!receiveData(client_socket, password, sizeof(password), &bytes_received))
    {
      printf("Error al recibir contraseña: %d\n", WSAGetLastError());
      disconnectClient(client_socket);
    }
    printf("Contraseña: %s\n", password);

    // Validar campos vacíos
    if (username[0] == '\0' || password[0] == '\0')
    {
      printf("Nombre de usuario o contraseña vacíos\n");
      if (!sendData(client_socket, "ERR", 4))
      {
        printf("Error al enviar error: %d\n", WSAGetLastError());
        disconnectClient(client_socket);
      }
      continue;
    }

    if (option == 0x02) // Registro
    {
      if (!existeUsuario(username))
      {
        insertarUsuarios(username, password);
        logroBienvenido(username);
        printf("Usuario registrado: %s\n", username);
        if (!sendData(client_socket, "ACK", 4))
        {
          printf("Error al enviar confirmación: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
        clienteConocido(client_socket, username);
      }
      else
      {
        printf("Usuario ya existe: %s\n", username);
        if (!sendData(client_socket, "ERR", 3))
        {
          printf("Error al enviar error1: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
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
          disconnectClient(client_socket);
        }
        clienteConocido(client_socket, username);
      }
      else
      {
        printf("Autenticación fallida: %s\n", username);
        if (!sendData(client_socket, "ERR", 4))
        {
          printf("Error al enviar error: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
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
      disconnectClient(client_socket);
    }

    printf("Opción seleccionada: %02X\n", selection);

    switch (selection)
    {
    case 0xE0: // Emular CHIP8
    {
      printf("Emulando CHIP8...\n");
      char selectedRom[256];
      if (!receiveData(client_socket, selectedRom, sizeof(selectedRom), &bytes_received))
      {

        printf("Error al recibir ROM: %d\n", WSAGetLastError());
        disconnectClient(client_socket);
      }
      printf("ROM seleccionada: %s\n", selectedRom);
      servirChip8(client_socket, selectedRom, username);

      // checkear la puntuacion acumulada para Chip8
      // estoy dando por hecho que se comprueba si tienes ya el logro o no
      if (getPuntosTotalesUsuario(username, "CHIP8") >= 1000)
      {

        logroChip8Fan(username);
      }
      else if (getPuntosTotalesUsuario(username, "CHIP8") >= 5000)
      {
        logroChip8Pro(username);
      }
      else if (getPuntosTotalesUsuario(username, "CHIP8") >= 10000)
      {
        logroChip8Master(username);
      }

      break;
    }

    case 0xE1: // Emular NES
    {
      printf("Emulando NES...\n");
      logroNesTester(username);
      char selectedRom[128];
      if (!receiveData(client_socket, selectedRom, sizeof(selectedRom), &bytes_received))
      {
        printf("Error al recibir ROM: %d\n", WSAGetLastError());
        disconnectClient(client_socket);
      }
      printf("ROM seleccionada: %s\n", selectedRom);
      servirNES(client_socket, selectedRom);
      // checkear la puntuacion acumulada para NES
      // checkear la puntuacion acumulada para Chip8
      // estoy dando por hecho que se comprueba si tienes ya el logro o no
      if (getPuntosTotalesUsuario(username, "NES") >= 1000)
      {

        logroNesFan(username);
      }
      else if (getPuntosTotalesUsuario(username, "NES") >= 5000)
      {
        logroNesPro(username);
      }
      else if (getPuntosTotalesUsuario(username, "NES") >= 10000)
      {
        logroNesMaster(username);
      }
      break;
    }

    case 0x00:
      printf("Cliente cierra sesion...\n");
      return;

    case 0x01: // Cambiar contraseña
    {
      printf("Cambiando contraseña...\n");
      // Recibir vieja contraseña
      if (!receiveData(client_socket, buffer, sizeof(buffer), &bytes_received))
      {
        printf("Error al recibir contraseña: %d\n", WSAGetLastError());
        disconnectClient(client_socket);
      }
      printf("Contraseña vieja: %s\n", buffer);
      if (!existeUsuarioYPas(username, buffer))
      {
        printf("Contraseña incorrecta\n");
        if (!sendData(client_socket, "ERR", 4))
        {
          printf("Error al enviar error: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
        continue;
      }
      else
      {
        printf("Contraseña correcta\n");
        if (!sendData(client_socket, "ACK", 4))
        {
          printf("Error al enviar confirmación: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }
      // Recibir nueva contraseña
      if (!receiveData(client_socket, buffer, sizeof(buffer), &bytes_received))
      {
        printf("Error al recibir nueva contraseña: %d\n", WSAGetLastError());
        disconnectClient(client_socket);
      }
      printf("Nueva contraseña: %s\n", buffer);
      if (!updateContrasena(buffer, username))
      {
        printf("Error al cambiar contraseña\n");
        if (!sendData(client_socket, "ERR", 4))
        {
          printf("Error al enviar error: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }
      else
      {
        printf("Contraseña cambiada con éxito\n");
        if (!sendData(client_socket, "ACK", 4))
        {
          printf("Error al enviar confirmación: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }
      break;
    }
    case 0x02: // Enviar ROMs CHIP8
    {
      printf("Enviando ROMs CHIP8...\n");
      char **romOptions; // Array para almacenar nombres de ROMs
      int romCount = 0;
      romCount = getJuegosDisponibles(&romOptions, "CHIP8");
      printf("Cantidad de ROMs: %d\n", romCount);

      if (!sendData(client_socket, (char *)&romCount, sizeof(romCount)))
      {
        printf("Error al enviar conteo ROMs: %d\n", WSAGetLastError());
        disconnectClient(client_socket);
      }
      for (int i = 0; i < romCount; i++)
      {
        if (!sendData(client_socket, romOptions[i], strlen(romOptions[i]) + 1))
        {
          printf("Error al enviar ROM: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }
      break;
    }
    case 0x03: // Enviar ROMs NES
      printf("Enviando ROMs NES...\n");
      char **romOptions; // Array para almacenar nombres de ROMs
      int romCount = 0;
      romCount = getJuegosDisponibles(&romOptions, "NES");
      printf("Cantidad de ROMs: %d\n", romCount);

      if (!sendData(client_socket, (char *)&romCount, sizeof(romCount)))
      {
        printf("Error al enviar conteo ROMs: %d\n", WSAGetLastError());
        disconnectClient(client_socket);
      }
      for (int i = 0; i < romCount; i++)
      {
        if (!sendData(client_socket, romOptions[i], strlen(romOptions[i]) + 1))
        {
          printf("Error al enviar ROM: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }
      break;
    case 0x11: // Ver Logros
      printf("Ver Logros..\n");
      char **nombreLogros = NULL;
      char **descripcionLogros = NULL;
      char **fechaObtencion = NULL;
      int cantidadLogros = 0;
      // Obtenemos la cantidad de logros y resto de datos
      cantidadLogros = getLogros(username, &nombreLogros, &descripcionLogros, &fechaObtencion);
      // enviar cantidad de logros
      if (!sendData(client_socket, &cantidadLogros, sizeof(cantidadLogros)))
      {
        printf("Error al enviar CantidadLogros: %d\n", WSAGetLastError());
        disconnectClient(client_socket);
      }
      // Enviar lista de nombres de logros
      for (int i = 0; i < cantidadLogros; i++)
      {
        if (!sendData(client_socket, nombreLogros[i], strlen(nombreLogros[i]) + 1))
        {
          printf("Error al enviar NombreLogro: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }
      // Enviar lista de descripciones de logros
      for (int i = 0; i < cantidadLogros; i++)
      {
        if (!sendData(client_socket, descripcionLogros[i], strlen(descripcionLogros[i]) + 1))
        {
          printf("Error al enviar DescripcionLogro: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }
      // Enviar lista de logros obtenidos
      for (int i = 0; i < cantidadLogros; i++)
      {
        if (!sendData(client_socket, fechaObtencion[i], strlen(fechaObtencion[i]) + 1))
        {
          printf("Error al enviar FechaObtencion: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }
      // Liberar memoria
      for (int i = 0; i < cantidadLogros; i++)
      {
        free(nombreLogros[i]);
        free(descripcionLogros[i]);
        free(fechaObtencion[i]);
      }
      free(nombreLogros);
      free(descripcionLogros);
      free(fechaObtencion);
      printf("Datos de logros enviados\n");
      break;

    case 0x12: // Ver Amigos
      printf("Ver Amigos...\n");
      int cantidadAmig = 0;
      char **nombreAmigos = NULL;
      int *cantidadAmigos = &cantidadAmig;

      // Obtenemos la cantidad de amigos y resto de datos
      nombreAmigos = getNombreAmigos(username, cantidadAmigos);

      if (!sendData(client_socket, &cantidadAmigos, sizeof(cantidadAmigos)))
      {
        printf("Error al enviar CantidadAmigos: %d \n", WSAGetLastError());
        disconnectClient(client_socket);
      }

      // Enviar la lista de Amigos

      for (int i = 0; i < cantidadAmigos; i++)
      {
        if (!sendData(client_socket, nombreAmigos[i], sizeof(nombreAmigos[i])))
        {
          printf("Error al enviar nombreAmigos: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }

      break;

    case 0x13: // Ver Tiempo Jugado
    {
      printf("Ver Tiempo Jugado...\n");

      char **nombreJuegos = NULL;
      int *tiemposSegundos = NULL;
      int cantidadJuegos = 0;

      // Obtenemos la cantidad de juegos y datos asociados
      cantidadJuegos = getTiempoJugadoTodosLosJuegos(username, &nombreJuegos, &tiemposSegundos);

      // enviar cantidad de juegos
      if (!sendData(client_socket, &cantidadJuegos, sizeof(cantidadJuegos)))
      {
        printf("Error al enviar CantidadJuegos: %d\n", WSAGetLastError());
        disconnectClient(client_socket);
      }

      // Enviar lista de nombres de juegos
      for (int i = 0; i < cantidadJuegos; i++)
      {
        if (!sendData(client_socket, nombreJuegos[i], sizeof(nombreJuegos[i])))
        {
          printf("Error al enviar NombreJuego: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }

      // Enviar lista de tiempos
      for (int i = 0; i < cantidadJuegos; i++)
      {
        if (!sendData(client_socket, &tiemposSegundos[i], sizeof(tiemposSegundos[i])))
        {
          printf("Error al enviar TiemposJuegos: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }
      free(nombreJuegos);
      free(tiemposSegundos);
      printf("Datos de tiempo jugado enviados\n");
      break;
    }

    case 0x14: // Ver partidas jugadas
    {
      printf("Ver Partidas Jugadas...\n");
      char nombreJuego[128];
      if (!receiveData(client_socket, nombreJuego, sizeof(nombreJuego), &bytes_received))
      {
        printf("Error al recibir NombreJuego: %d\n", WSAGetLastError());
        disconnectClient(client_socket);
      }
      printf("NombreJuego: %s\n", nombreJuego);

      char **partidas = NULL;
      int *tiemposJugados = NULL;
      int *puntuacionesMaximas = NULL;
      int cantidadPartidas = 0;

      // Obtener partidas jugadas
      cantidadPartidas = getPartidasDeJuego(username, nombreJuego, &partidas, &tiemposJugados, &puntuacionesMaximas);

      // Enviar cantidad de partidas
      if (!sendData(client_socket, &cantidadPartidas, sizeof(cantidadPartidas)))
      {
        printf("Error al enviar CantidadPartidas: %d\n", WSAGetLastError());
        disconnectClient(client_socket);
      }

      // Enviar lista de partidas
      for (int i = 0; i < cantidadPartidas; i++)
      {
        if (!sendData(client_socket, partidas[i], sizeof(partidas[i])))
        {
          printf("Error al enviar Partida: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }

      // Enviar lista de tiempos jugados
      for (int i = 0; i < cantidadPartidas; i++)
      {
        if (!sendData(client_socket, &tiemposJugados[i], sizeof(tiemposJugados[i])))
        {
          printf("Error al enviar TiemposJugados: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }

      // Enviar lista de puntuaciones maximas
      for (int i = 0; i < cantidadPartidas; i++)
      {
        if (!sendData(client_socket, &puntuacionesMaximas[i], sizeof(puntuacionesMaximas[i])))
        {
          printf("Error al enviar PuntuacionesMaximas: %d\n", WSAGetLastError());
          disconnectClient(client_socket);
        }
      }

      // Liberar memoria
      for (int i = 0; i < cantidadPartidas; i++)
      {
        free(partidas[i]);
      }
      free(partidas);
      free(tiemposJugados);
      free(puntuacionesMaximas);
      printf("Datos de partidas jugadas enviados\n");
      break;
    }

    default:
    {
      printf("Opción no válida\n");
      disconnectClient(client_socket);
    }
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

void servirChip8(socket_t sock, char *selectedRom, char *username)
{
  logroChip8tester(username);

  if (strstr(selectedRom, "Pong") != NULL)
  {
    logroPingPong(username);
  }
  else if (strstr(selectedRom, "Cave") != NULL)
  {
    logroBuscandoLaSalida(username);
  }
  else if (strstr(selectedRom, "Tetris") != NULL)
  {
    logroTetris(username);
  }

  char fullRomPath[512];

  sprintf(fullRomPath, "resources/chip8-roms/%s", selectedRom);

  // Verificar si el archivo ROM existe
  FILE *file = fopen(fullRomPath, "rb");
  if (file == NULL)
  {
    printf("Error: No se pudo abrir la ROM %s\n", fullRomPath);
    return;
  }
  fclose(file);

  printf("ROM recibida en el servidor: %s (longitud: %zu)\n", fullRomPath, strlen(fullRomPath));
  Chip8 *chip8 = chip8init(fullRomPath);
  if (chip8 == NULL) // Validar si la inicialización falló
  {
    printf("Error: No se pudo inicializar el emulador CHIP8 con la ROM %s\n", fullRomPath);
    return;
  }

  // Enviar si es SuperChip8 o no
  uint8_t isSuperChip8 = 0;
  if (strstr(selectedRom, ".ch8") != NULL)
  {
    isSuperChip8 = 0;
  }
  else if (strstr(selectedRom, ".s8") != NULL) // nose si esta bien
  {
    isSuperChip8 = 1;
  }
  if (!sendData(sock, &isSuperChip8, sizeof(isSuperChip8)))
  {
    printf("Error al enviar el modo SuperChip8: %d\n", WSAGetLastError());
    return;
  }

  while (1)
  {
    // Lógica de emulación CHIP8

    chip8step(chip8);

    uint8_t exit_signal = 0; // TODO: hacer que se ponga 1 cuando el server lo pida (crash)
    if (!sendData(sock, &exit_signal, sizeof(exit_signal)))
    {
      printf("Error al enviar señal de salida: %d\n", WSAGetLastError());
      break;
    }

    // Enviar datos de pantalla al cliente
    if (!sendData(sock, chip8->pantalla, sizeof(uint8_t) * SCREEN_WIDTH_CHIP8 * SCREEN_HEIGHT_CHIP8))
    {
      printf("Error al enviar datos de pantalla: %d\n", WSAGetLastError());
      break;
    }

    if (!receiveData(sock, &exit_signal, sizeof(exit_signal), &bytes_received))
    {
      printf("Error al recibir señal de salida: %d\n", WSAGetLastError());
      break;
    }

    if (exit_signal == 0x01)
    {
      printf("Señal de salida recibida del cliente\n");
      break;
    }

    if (!receiveData(sock, chip8->teclado, sizeof(uint8_t) * 16, &bytes_received))
    {
      printf("Error al recibir datos del teclado: %d\n", WSAGetLastError());
      break;
    }

    // Enviar audio timer al cliente
    if (!sendData(sock, &chip8->sound_timer, sizeof(chip8->sound_timer)))
    {
      printf("Error al enviar audio timer: %d\n", WSAGetLastError());
      break;
    }

    // Lógica de emulación CHIP8
    // sleep_ms(16); // ~60 FPS
  }

  printf("INFO: CHIP8 emulation finished\n");
  chip8terminate(chip8, username);
}

void servirNES(socket_t sock, char *selectedRom)
{
  // Verificar si el archivo ROM existe
  char fullRomPath[512];
  sprintf(fullRomPath, "resources/nes-roms/%s.nes", selectedRom);
  FILE *file = fopen(fullRomPath, "rb");
  if (file == NULL)
  {
    printf("Error: No se pudo abrir la ROM %s\n", fullRomPath);
    return;
  }
  fclose(file);

  printf("ROM recibida en el servidor: %s (longitud: %zu)\n", fullRomPath, strlen(fullRomPath));

  // Inicializar emulador NES
  nes_launch(sock, fullRomPath);
  printf("INFO: NES emulation finished\n");
  return;
}