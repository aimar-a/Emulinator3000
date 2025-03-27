#include "menu_cmd.h"
#include "bd.h"
#include <stdbool.h>
#include <windows.h> //para usar "sleep" (funciona en windows en linux/mac es diferente)

#define ROMS_PATH "resources/chip8-roms"

void clearScreen()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

char * currentUser;

void menuUsuario(){

  //opciones: 
  //  1.Iniciar sesion --> Lleva a otro menu donde poner Usuario y contraseña 
  //  2.Regsitrar --> Lleva a otro menu donde poner NUEVOUSUARIO y NUEVACONTRASEÑA

  // 4.En MenuInicial meter opcion de cambiar Contraseña
  // 5. el SALIR  de menuInicial tiene que ir a menuUsuario

  bool pass = false;

  while(pass==false){

    char buffer1[3];
    
    clearScreen();
    printf("Seleccione una de las siguientes opciones\n");
    printf("\n1.INICIAR SESIÓN: \n");
    printf("\n2.REGISTRARSE: \n");
    printf("\n0. Salir\n");
    printf("\nOpción: ");

    fgets(buffer1, sizeof(buffer1), stdin);

    if(buffer1[0]=='1'){
      clearScreen();
      printf("\nIntroduce tu Usuario: ");
      char buffer2[22];
      fgets(buffer2, sizeof(buffer2), stdin);
      buffer2[strcspn(buffer2, "\n")] = '\0';  // Eliminar el salto de línea al final

      printf("\nIntroduce tu Contraseña: ");
      char buffer3[22];
      fgets(buffer3, sizeof(buffer3), stdin);
      buffer3[strcspn(buffer3, "\n")] = '\0';  // Eliminar el salto de línea al final

  
      //implementacion BD (comprobar si existe un usuario con esa contraseña)
      if(existeUsuarioYPas(buffer2, buffer3)==true){
        pass = true;
        printf("\nhas accedido correctamente");
        currentUser=buffer2;
        clearScreen();
        menuInicial();
        break;
      }else{

        printf("\nUsuario o contraseña incorrectos");
        Sleep(1000);

      }
      

    } else if (buffer1[0]=='2'){
      clearScreen();
      printf("\nUsuario: ");
      char buffer4[22];
      fgets(buffer4, sizeof(buffer4), stdin);
      buffer4[strcspn(buffer4, "\n")] = '\0';  // Eliminar el salto de línea al final

      printf("\nContraseña: ");
      char buffer5[22];
      fgets(buffer5, sizeof(buffer5), stdin);
      buffer5[strcspn(buffer5, "\n")] = '\0';  // Eliminar el salto de línea al final

      



      //implementacion BD (insertar usuario)
      if(buffer4[0]!='\0' && buffer5[0]!='\0make'){

        if(existeUsuario(buffer4)!=true){
          currentUser=buffer4;
          insertarUsuarios(buffer4, buffer5);
          printf("\nRegistro Exitoso");
          Sleep(1000);
          clearScreen();
          menuInicial();
          break;
        }else{
          printf("\nEse nombre de usuario ya existe, prueba a poner otro nombre");
          Sleep(500);
          printf("\nVolviendo al menu....");
          Sleep(1000);         
          clearScreen();
          
        }

      }else{

        printf("\nNo se pueden guardar como usuario o contraseña valores vacios");
        Sleep(500);
        printf("\nVolviendo al menu....");
        Sleep(1000);      
        clearScreen();
      }

  

    } else if (buffer1[0]=='0'){

      printf("\nCerrando....");
      Sleep(1000);
      break;

    }


  }}

void menuInicial()
{
  char option = 'w';
  while (option != '0')
  {
    option = ' ';
    clearScreen();
    printf("Estas jugando con el usuario: %s\n", currentUser);
    printf("4. Cambiar contraseña del usuario\n");
    printf("Bienvenido al emulador EMULINATOR 3000\n");
    printf("Seleccione la consola que desea emular:\n");
    printf("1. CHIP-8\n");
    printf("2. Game Boy\n");
    printf("3. NES\n");
    printf("0. Salir\n");
    printf("Opción: ");

    scanf(" %c", &option);

    switch (option)
    {
    case '1':
      showInitialWindow();
      break;
    case '2':
      printf("Emulando Game Boy...\n");
      break;
    case '3':
      nes_launch();
      break;
    case '0': 
      clearScreen();
      printf("\nVolviendo al menu de Inicio de sesion o Registro....");
      Sleep(1000);
      menuUsuario();
      break;
    default:
      printf("Opción inválida\n");
      break;
    }
  }
}

void menuChip8()
{
  char option = 'w';
  while (option != '0')
  {
    option = ' ';
    clearScreen();
    printf("CHIP-8\n");
    printf("Seleccione una opción:\n");
    printf("1. Cargar ROM\n");
    printf("2. Configuración\n");
    printf("0. Volver\n");
    printf("Opción: ");

    scanf(" %c", &option);

    switch (option)
    {
    case '1':
      menuListaROMs();
      break;
    case '2':
      printf("Configurando...\n");
      break;
    case '0':
      printf("Volviendo...\n");
      break;
    default:
      printf("Opción inválida\n");
      break;
    }
  }
}

void listarROMsRecursivo(const char *directory, char roms[][256], int *count)
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
          snprintf(roms[*count], 256, "%s", path);
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

void menuListaROMs()
{
  char roms[200][256];
  int romCount = 0;

  printf("Listado de ROMs disponibles:\n");
  listarROMsRecursivo(ROMS_PATH, roms, &romCount);

  if (romCount == 0)
  {
    printf("No se encontraron ROMs.\n");
    return;
  }

  int pagina = 0;
  int totalPaginas = (romCount + 19) / 20;
  char opcion;

  do
  {
    opcion = ' ';
    clearScreen();
    printf("Listado de ROMs (Página %d de %d):\n", pagina + 1, totalPaginas);

    for (int i = pagina * 20; i < (pagina + 1) * 20 && i < romCount; i++)
    {
      printf("%d. %s\n", i + 1, roms[i]);
    }

    printf("\nOpciones:\n");
    printf("-1. Página anterior\n");
    printf("-2. Página siguiente\n");
    printf("0. Salir\n");
    printf("Seleccione un ROM para ejecutar (1-%d) o navegar (-1, -2): ", romCount);
    scanf(" %c", &opcion);

    if (opcion == '-1' && pagina > 0)
    {
      pagina--;
    }
    else if (opcion == '-2' && pagina < totalPaginas - 1)
    {
      pagina++;
    }
    else if (opcion >= '1' && opcion <= '9')
    {
      int seleccion = opcion - '0';
      if (seleccion > 0 && seleccion <= romCount)
      {
        chip8cpuLaunch(roms[seleccion - 1]);
        return;
      }
    }
  } while (opcion != '0');
}
