#include <stdio.h>
#include "juego.h"
#include "zombies.h"
void actualizarTurno(int**jardin, Zombie zombies[], int cantidadZombies, int *soles, int *contadorGirasoles, int *juegoTerminado){

        //contamos los girasoles que hay en el jardin
        int cantidadGirasoles=0;

        for(int i=0; i<5; i++){
            for(int j=0; j<9; j++){
                if(jardin[i][j]==2){
                    cantidadGirasoles++;
                }
            }
        }
        //contamos los turnos para generar soles
        (*contadorGirasoles)++;
        if(*contadorGirasoles==2 && cantidadGirasoles>0){
            //agregamos 50 soles por cada girasol
            *soles=*soles+(50*cantidadGirasoles);

            printf("Los girasoles generaron %d soles\n", 50*cantidadGirasoles);
            printf("cantidad de soles: %d\n", *soles);

            //reiniciamos el contador
            *contadorGirasoles=0;
        }
            //recorremos todos los zombies
            for(int i=0; i<cantidadZombies; i++){

                //verificamos si el zombie esta activo
                if(zombies[i].activo==1){

                    //contamos los turnos para que las plantas ataquen
                    zombies[i].contadorAtaque++;

                    if(zombies[i].contadorAtaque==2){

                        //recorremos la fila donde esta el zombie
                        for(int j=0; j<zombies[i].columna; j++){

                            //verificamos si hay lanzaguisantes
                            if(jardin[zombies[i].fila][j]==1){

                                //le quitamos vida al zombie
                                zombies[i].vida--;

                                printf("Zombie %d recibe un ataque. Vida %d\n", i+1, zombies[i].vida);

                                //verificamos si fue derrotado
                                if(zombies[i].vida<=0){
                                    break;
                                }
                            }
                        }

                        //reiniciamos el contador de ataque
                        zombies[i].contadorAtaque=0;
                    }
                    //verificamos si el zombies fue derrotado 
                    if(zombies[i].vida<=0){

                        //eliminamos al zombie de la matriz
                        jardin[zombies[i].fila][zombies[i].columna]=0;

                        //inicamos que ya no este activo
                        zombies[i].activo=0;

                        printf("Zombie %d fue derrotado\n",  i+1);
                    }
                    else{

                        //verificamos si llego a la casa
                        if(zombies[i].columna==0){
                            printf("Zombie %d llego a la casa, Perdiste :(\n", i+1);

                            *juegoTerminado=1;
                            return;
                        }
                        //movemos el zombie una posicion
                        moverZombie(jardin, &zombies[i]);
                    }
                }
            }
            //contamos cuantos zombies siguen activos
            int zombiesActivos=0;

            for(int i=0; i<cantidadZombies; i++){
                if(zombies[i].activo==1){
                    zombiesActivos++;
                }
            }
            //verificamos si derrotamos a todos los zombies
            if(zombiesActivos==0){
                printf("derrotaste a todos los zombies. Ganaste :) \n");
                *juegoTerminado=1;
            }
         }
