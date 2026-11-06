#include <stdio.h>
#include "zombies.h"

//colocamos el zombie en el jardin
void colocarZombie(int**jardin, int fila, int columna){
    jardin[fila][columna]=4;
}

void moverZombie(int **jardin, int fila, int *columna){

    // borramos la posicion anterior del zombie
    jardin[fila][*columna]=0;

    // cambiamos la columna para moverlo una posicion a la izquierda
    *columna=*columna-1;

    // colocamos el zombie en su nueva posicion
    jardin[fila][*columna]=4;
}