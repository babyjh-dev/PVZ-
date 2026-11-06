#include <stdio.h>
#include "juego.h"
#include "zombies.h"
void actualizarTurno(int**jardin, int fila,int filaPlanta, int*columnaZombie, int columnaPlanta,int *vidaZombie,int *contadorAtaque, int *soles, int girasolComprado, int *contadorGirasoles, int *juegoTerminado){

            // verificamos si compramos un girasol
            if(girasolComprado==1){

                // contamos los turnos para generar soles
                (*contadorGirasoles)++;

                if(*contadorGirasoles==2){

                    // agregamos 50 soles
                    *soles=*soles+50;

                    printf("El girasol genero 50 soles!\n");
                    printf("Cantidad de soles: %d\n",*soles);

                    // reiniciamos el contador
                    *contadorGirasoles=0;
                }
            }

            // contamos los turnos para que la planta ataque
            (*contadorAtaque)++;

            if(*contadorAtaque==2 && *columnaZombie>0){

                // verificamos si hay un lanzaguisantes en la fila del zombie
                if(columnaPlanta>0 && columnaPlanta>=0 && jardin[fila][columnaPlanta]==1&& filaPlanta==fila){
                    // le quitamos vida al zombie
                    (*vidaZombie)--;

                    printf("La planta ataca! Vida del zombie: %d\n", *vidaZombie);
                }

                // reiniciamos el contador de ataque
                *contadorAtaque=0;
            }

            // verificamos si el zombie fue derrotado
            if(*vidaZombie<=0){

                // eliminamos el zombie de la matriz
                jardin[fila][*columnaZombie]=0;

                printf("El zombie fue derrotado. Ganaste!");

                // indicamos que el juego termino
                *juegoTerminado=1;
            }
            else{

                // movemos el zombie una posicion
                moverZombie(jardin, fila, columnaZombie);

                // verificamos si el zombie llego a la casa
                if(*columnaZombie==0){

                    printf("El zombie llego a la casa. Perdiste!");

                    // indicamos que el juego termino
                    *juegoTerminado=1;
                }
            }
        }
