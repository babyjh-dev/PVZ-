#ifndef jardin_h
#define jardin_h

#include "zombies.h"

// Crea una matriz dinamica para representar el jardin
int **crearJardin(int *filas, int *columnas);

// Lee las dimensiones y el contenido inicial del jardin desde un archivo
int **leerJardin(
    const char *nombreArchivo,
    int *filas,
    int *columnas
);

// Imprime el contenido de la matriz en la consola
void imprimirMatriz(int **jardin, int *filas, int *columnas);

// Dibuja el jardin y los zombies en pantalla
void dibujarJardin(
    int **jardin,
    int *filas,
    int *columnas,
    Zombie *zombies,
    int *cantidadZombies
);

// Libera la memoria dinamica utilizada por el jardin
void liberarJardin(int **jardin, int *filas);

#endif
