#ifndef zombies_h
#define zombies_h

#include "estructuras.h"

// Definimos la cantidad maxima de zombies
#define MAX_ZOMBIES 25

// Coloca un zombie en una posicion del jardin
void colocarZombie(int **jardin, int fila, int columna);

// Inicializa los datos y caracteristicas de un zombie
void inicializarZombie(Zombie *zombie, int fila, int columna, ZombieConfig *configuracion);

// Genera los zombies correspondientes a cada oleada
void generarOleadas(
    int **jardin,
    Zombie *zombies,
    int *cantidadZombies,
    int *zombiesGenerados,
    int *zombiesPorOleada,
    int *capacidadZombies,
    int *filas,
    int *columnas,
    ZombieConfig *configuraciones,
    int *cantidadTipos
);

// Mueve al zombie por el jardin y permite que ataque las plantas
void moverZombie(int **jardin, Zombie *zombie);

// Activa el cortacesped para eliminar los zombies de una fila
void activarCortacesped(
    int **jardin,
    int fila,
    int *cortacesped,
    int columnas
);

#endif
