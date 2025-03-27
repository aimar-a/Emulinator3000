#ifndef _BD_H_
#define _BD_H_

#include <stdbool.h>
void crearBD();

// Funciones para insertar datos en las tablas
void insertarUsuarios(char* user, char* contraseña);
void insertarPartida(char* user, int idjuego, int tiempojugado, int puntmax);
void insertarJuego(char* titulo, char* rom, int puntrecord, char* usuariorecord);
void insertarTiempoJugado(int tiempojugado, char* user, int idjuego);
void insertarLogros(char* nombre, char* descripcion, int idjuego);
void insertarLogrosUsuarios(char* user, int idlogro, char* fecha );
void insertarAmigos(char* user1, char* user2, char* estado);

// Funciones para actualizar los datos en las tablas
void updateTiempoJugado(int tiempoJugado, char* user, int id_juego);
void updateEstado_Amigos(char* user1, char* user2, char* estado);
void updateContrasena(char* newcontrasena, char* user);
void updateUsuarioRecord(char* usuario, int idjuego);
void updatePuntuacionRecord(int newPunt, int idjuego);

//mas funciones
bool existeUsuarioYPas(char* name, char* pass);
bool existeUsuario(char* name);


#endif 