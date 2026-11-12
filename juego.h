#ifndef juego_h
#define juego_h

#include "zombies.h"

//actualizamos los tuenos del juego 
void actualizarTurno(int**jardin, Zombie zombies[], int cantidadZombies, int *soles, int *contadorGirasoles, int *juegoTerminado, int cortacesped[]);
//jardin matriz 5x9, zombies[]contiene los datos dde todos los zombies, cantidad de zombiess, soles cantiad de soles disponibles

#endif
