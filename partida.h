#ifndef PARTIDA_H
#define PARTIDA_H

#include "estructuras.h"

/* Guarda el estado actual de una partida en un archivo. */
int guardarPartida(const char *nombreArchivo, Partida *partida);

/* Carga una partida desde un archivo. */
int cargarPartida(const char *nombreArchivo, Partida *partida);

#endif
