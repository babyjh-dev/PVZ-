#include <stdio.h>
#include "zombies.h"

//colocamos el zombie en el jardin
void colocarZombie(int**jardin, int fila, int columna){
    jardin[fila][columna]=4;
}
// movemos el zombie
void moverZombie(int **jardin, Zombie *zombie){

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
        else{

            // borramos la posicion anterior
            jardin[zombie->fila][zombie->columna]=0;

            // avanzamos una columna
            zombie->columna--;

            // colocamos el zombie en su nueva posicion
            jardin[zombie->fila][zombie->columna]=4;

            // reiniciamos el contador de comida
            zombie->contadorComida=0;
        }
    }
}
