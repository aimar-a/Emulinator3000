#ifndef _BD_H_
#define _BD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdbool.h>
#include "config_file.h"
#include "directories.h"

void crearBD();

int iniciarBaseDeDatos();

// Funciones para insertar datos en las tablas
int abrirBaseDeDatos(sqlite3 **db);
void limpiarBaseDeDatos();
void eliminarBaseDeDatos();
void insertarUsuarios(char *user, char *contraseña);
void insertarPartida(char *user, int idjuego, int puntmax, char *fechaInicio, char *fechaFin);
void cargarJuegos();

void insertarJuego(char *titulo, char *rom);
void insertarTiempoJugado(int tiempojugado, char *user, int idjuego);
void insertarLogros(int idlogro, char *nombre, char *descripcion);
void insertarLogrosUsuarios(char *user, int idlogro, char *fecha);
void insertarAmigos(char *user1, char *user2, char *estado);

// Funciones para actualizar los datos en las tablas
void updateTiempoJugado(int tiempoJugado, char *user, int id_juego);
void updateEstado_Amigos(char *user1, char *user2, char *estado);
bool updateContrasena(char *newcontrasena, char *user);
void updateUsuarioPuntuacionRecord(char *usuario, int newPunt, int idjuego);

// mas funciones
bool existeUsuarioYPas(char *name, char *pass);
bool existeUsuario(char *name);
bool comprobarContraseña(char *user, char *password);
int getIdJuego(char *romjuego);
bool hajugado(char *user, int id_juego);
int getTiempoJugado(char *user, int idJuego);

int getPuntuacionRecord(int idJuego);
int getTiempoJugadoTodosLosJuegos(char *user, char ***nombreJuegos, int **tiempos);
int getPartidasDeJuego(char *user, char *nombreJuego, char ***partidas, int **tiemposJugados, int **puntuacionesMaximas);
int getNombreJuegos(char *user, char ***nombreJuegos);

int getJuegosDisponibles(char ***nombreJuegos);
char **getNombreAmigos(char *user, int *cantidad);

int getLogros(char *user, char ***nombreLogros, char ***descripcionLogros, char ***fechaObtencion);

// Datos de prueba de CSV
void cargarSampleData();
void cargarPartidasDeCSV(char *nombreArchivo);
void cargarLogrosUsuarioDeCSV(char *nombreArchivo);
void cargarAmigosDeCSV(char *nombreArchivo);
void cargarTiempoJugadoDeCSV(char *nombreArchivo);
void cargarLogrosDeCSV(char *nombreArchivo);
void cargarUsuariosDeCSV(char *nombreArchivo);

#endif