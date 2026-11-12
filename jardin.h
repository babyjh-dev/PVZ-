#ifndef jardin_h
#define jardin_h
#include "zombies.h"

int**crearJardin(int filas, int columnas);
int leerJardin(const char *nombreArchivo, int ***jardin, int *filas, int *columnas);
void imprimirMatriz(int**jardin, int filas, int columnas);
void dibujarJardin(int**jardin, int filas, int columnas, Zombie zombies[], int cantidadZombies);
void liberarJardin(int**jardin, int filas);

#endif
