
#include <stdio.h>
#include "juego.h"
#include "zombies.h"

// Actualizamos el estado del juego
void actualizarTurno(
    int **jardin,
    Zombie zombies[],
    int cantidadZombies,
    int *soles,
    int *contadorGirasoles,
    int *juegoTerminado,
    int cortacesped[],
    int filas,
    int columnas,
    int **contadoresAtaque
){

    // Contamos los girasoles que hay en el jardin
    int cantidadGirasoles = 0;

    for(int i = 0; i < filas; i++){
        for(int j = 0; j < columnas; j++){
            if(jardin[i][j] == CELDA_GIRASOL){
                cantidadGirasoles++;
            }
        }
    }

    // Contamos los turnos para generar soles
    (*contadorGirasoles)++;

    if(*contadorGirasoles == 2 && cantidadGirasoles > 0){

        // Agregamos 50 soles por cada girasol
        *soles = *soles + (50 * cantidadGirasoles);

        printf("Los girasoles generaron %d soles\n",
               50 * cantidadGirasoles);

        printf("Cantidad de soles: %d\n", *soles);

        // Reiniciamos el contador
        *contadorGirasoles = 0;
    }

    // Recorremos las plantas que pueden atacar
    for(int i = 0; i < filas; i++){
        for(int j = 0; j < columnas; j++){

            int dano = 0;
            float frecuencia = 0;

            // Identificamos el tipo de planta
            if(jardin[i][j] == CELDA_LANZAGUISANTES){
                dano = 20;
                frecuencia = 1.5;
            }
            else if(jardin[i][j] == CELDA_GATOESPINOSO){
                dano = 30;
                frecuencia = 3.0;
            }
            else if(jardin[i][j] == CELDA_LANZAGUISANTESFUEGO){
                dano = 40;
                frecuencia = 1.5;
            }

            // Verificamos si la planta puede atacar
            if(dano > 0){

                // Contamos los turnos de recarga
                contadoresAtaque[i][j]++;

                if(contadoresAtaque[i][j] >= frecuencia){

                    int zombieCercano = -1;
                    int columnaCercana = columnas;

                    // El gato ataca a todos los zombies de su fila
                    if(jardin[i][j] == CELDA_GATOESPINOSO){

                        for(int k = 0; k < cantidadZombies; k++){

                            if(zombies[k].activo == 1 &&
                               zombies[k].fila == i &&
                               zombies[k].columna > j){

                                zombies[k].vida -= dano;

                                printf("El gato espinoso ataco al zombie %d. Vida: %d\n",
                                       k + 1, zombies[k].vida);

                                if(zombies[k].vida <= 0){

                                    jardin[zombies[k].fila][zombies[k].columna] = CELDA_VACIA;
                                    zombies[k].activo = 0;

                                    printf("Zombie %d fue derrotado\n", k + 1);
                                }
                            }
                        }
                    }
                    else{

                        // Buscamos el zombie mas cercano de la fila
                        for(int k = 0; k < cantidadZombies; k++){

                            if(zombies[k].activo == 1 &&
                               zombies[k].fila == i &&
                               zombies[k].columna > j &&
                               zombies[k].columna < columnaCercana){

                                zombieCercano = k;
                                columnaCercana = zombies[k].columna;
                            }
                        }

                        // Atacamos al zombie encontrado
                        if(zombieCercano != -1){

                            zombies[zombieCercano].vida -= dano;

                            if(jardin[i][j] == CELDA_LANZAGUISANTESFUEGO){
                                printf("El lanzaguisantes de fuego ataco al zombie %d. Vida: %d\n",
                                       zombieCercano + 1,
                                       zombies[zombieCercano].vida);
                            }
                            else{
                                printf("El lanzaguisantes ataco al zombie %d. Vida: %d\n",
                                       zombieCercano + 1,
                                       zombies[zombieCercano].vida);
                            }

                            if(zombies[zombieCercano].vida <= 0){

                                jardin[zombies[zombieCercano].fila][zombies[zombieCercano].columna] = CELDA_VACIA;
                                zombies[zombieCercano].activo = 0;

                                printf("Zombie %d fue derrotado\n", zombieCercano + 1);
                            }
                        }
                    }

                    // Reiniciamos el contador de ataque
                    contadoresAtaque[i][j] = 0;
                }
            }
        }
    }

    // Recorremos todos los zombies
    for(int i = 0; i < cantidadZombies; i++){

        // Verificamos si el zombie esta activo
        if(zombies[i].activo == 1){

            // Verificamos si el zombie fue derrotado
            if(zombies[i].vida <= 0){

                jardin[zombies[i].fila][zombies[i].columna] = CELDA_VACIA;
                zombies[i].activo = 0;

                printf("Zombie %d fue derrotado\n", i + 1);
            }
            else{

                // Verificamos si llego a la casa
                if(zombies[i].columna == 0){

                    if(cortacesped[zombies[i].fila] == 0){

                        printf("Un zombie llego a la casa. Perdiste :(\n");
                        *juegoTerminado = 1;
                        return;
                    }

                    activarCortacesped(
                        jardin,
                        zombies[i].fila,
                        &cortacesped[zombies[i].fila],
                        columnas
                    );

                    // Eliminamos todos los zombies de esa fila
                    for(int j = 0; j < cantidadZombies; j++){

                        if(zombies[j].activo == 1 &&
                           zombies[j].fila == zombies[i].fila){

                            zombies[j].activo = 0;
                        }
                    }

                    continue;
                }

                // Movemos el zombie una posicion
                moverZombie(jardin, &zombies[i]);
            }
        }
    }
}
