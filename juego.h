#ifndef juego_h
#define juego_h

#include "zombies.h"

void actualizarTurno(
    int **jardin,
    Zombie zombies[],
    int cantidadZombies,
    int *soles,
    int *contadorGirasoles,
    int *juegoTerminado,
    int cortacesped[],
    int filas,
    int columnas,
    int **contadoresAtaque
);

#endif
