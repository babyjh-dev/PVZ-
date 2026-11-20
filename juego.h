
#ifndef juego_h
#define juego_h

#include "estructuras.h"
#include "zombies.h"

// Actualizamos el estado del juego
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
    int **contadoresAtaque,
    Sol solesGenerados[],
    int *cantidadSoles,
    int *capacidadSoles
);

#endif
