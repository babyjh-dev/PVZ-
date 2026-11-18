#ifndef zombies_h
#define zombies_h

#include "estructuras.h"

#define MAX_ZOMBIES 25

void colocarZombie(int **jardin, int fila, int columna);

void inicializarZombie(Zombie *zombie, int fila, int columna, ZombieConfig configuracion);

void generarOleadas(
    int **jardin,
    Zombie zombies[],
    int *cantidadZombies,
    int *zombiesGenerados,
    int zombiesporOlead,
    int capacidadZombies,
    int filas,
    int columnas,
    ZombieConfig configuraciones[],
    int cantidaTipos
);

void moverZombie(int **jardin, Zombie *zombie);

void activarCortacesped(
    int **jardin,
    int fila,
    int *cortacesped,
    int columnass
);

#endif
