#include <stdio.h>
#include <stdlib.h>
#include "zombies.h"

//colocamos el zombie en el jardin
void colocarZombie(int**jardin, int fila, int columna){
    jardin[fila][columna]=4;
}
// inicializamos los datos de un zombie
void inicializarZombie(Zombie *zombie, int fila, int columna){

    zombie->fila=fila;
    zombie->columna=columna;
    zombie->vida=3;
    zombie->activo=1;
    zombie->contadorAtaque=0;
    zombie->contadorComida=0;
    zombie->contadorMovimiento=0;

    // elegimos aleatoriamente el tipo
    zombie->tipoZombie=rand()%2;
}
// generamos los zombies de una oleada
void generarOleadas(int **jardin, Zombie zombies[], int *cantidadZombies, int cantidadNueva, int filas){

    int fila;
    int columna;

    for(int i=0; i<cantidadNueva; i++){
        // buscamos una posicion disponible
        fila=rand()%filas;
        columna=8;
        // si la casilla esta ocupada, buscamos otra columna
        while(jardin[fila][columna]!=0){
            columna--;
            // si llegamos al inicio, volvemos a la ultima columna
            if(columna<0){
                columna=8;
                fila=(fila+1)%filas;
            }
        }
        // inicializamos el zombie
        inicializarZombie(&zombies[*cantidadZombies], fila, columna);
        // lo colocamos en el jardin
        colocarZombie(jardin, fila, columna);
        // aumentamos la cantidad de zombies
        (*cantidadZombies)++;
    }
}

// movemos el zombie
void moverZombie(int **jardin, Zombie *zombie){
    //definimos la velocidad del zombie
    int velocidad=2;

    //el abanderaddo avanza mas rapido
    if(zombie->tipoZombie==1){
        velocidad=1;
    }
    //contamos los turnos de movimiento
    zombie->contadorMovimiento++;

    if(zombie->contadorMovimiento<velocidad){
        return;
    }
    zombie->contadorMovimiento=0;

    // verificamos que el zombie este activo y pueda avanzar
    if(zombie->activo==1 && zombie->columna>0){

        // revisamos la casilla que esta delante del zombie
        int siguienteColumna=zombie->columna-1;

        // verificamos si hay una planta
        if(jardin[zombie->fila][siguienteColumna]>=1 &&
           jardin[zombie->fila][siguienteColumna]<=3){

            // el zombie se detiene para comer la planta
            zombie->contadorComida++;

            printf("el zombie se esta comiendo una planta\n");

            // definimos cuantos turnos resiste la planta
            int resistencia=2;

            // si es una nuez, resiste 5 turnos
            if(jardin[zombie->fila][siguienteColumna]==3){
                resistencia=5;
            }

            // verificamos si termino de comerse la planta
            if(zombie->contadorComida>=resistencia){

                // eliminamos la planta
                jardin[zombie->fila][siguienteColumna]=0;

                printf("el zombie destruyo la planta\n");

                // reiniciamos el contador
                zombie->contadorComida=0;
            }
        }
        else if(jardin[zombie->fila][siguienteColumna]==4){
            //si hay otro zombie adelante, esperamos 
            return;
        }
        else {
            jardin[zombie->fila][zombie->columna]=0;
            zombie->columna--;
            jardin[zombie->fila][zombie->columna]=4;
            zombie->contadorComida=0;
        }
    }
}
void activarCortacesped(int**jardin, int fila, int *cortacesped){

    //verificamos si el cortacesped esta disponible
    if(*cortacesped==1){
        printf("el cortacesped de la fila %d se activo \n", fila+1);

        //eliminamos los zombies de esa fila
        for(int j=0; j<9; j++){
            if(jardin[fila][j]==4){
                jardin[fila][j]=0;
            }
        }
        //indicamos que ya fue usado
        *cortacesped=0;
    }
}
