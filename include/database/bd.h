#ifndef _BD_H_
#define _BD_H_

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
void actualizarTiempoJugado(int tiempoJugado, char* user, int id_juego);
void actualizarEstado_Amigos(char* user1, char* user2, char* estado);

#endif 