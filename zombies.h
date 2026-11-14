#ifndef zombies_h
#define zombies_h

#include"estructuras.h"
#define MAX_ZOMBIES 25

//funciones de los zombies
void colocarZombie(int**jardin, int fila, int columna);
void inicializarZombie(Zombie *zombie, int fila, int columna);
void generarOleadas(int **jardin, Zombie zombies[], int *cantidadZombies, int cantidadNueva, int filas);
void moverZombie(int**jardin, Zombie *zombie);
void activarCortacesped(int**jardin, int fila, int *cortacesped);
#endif
