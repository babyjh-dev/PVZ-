#include <stdio.h>
#include <stdlib.h>

#include "juego.h"
#include "zombies.h"
#include "estructuras.h"

// Contador para generar soles aleatorios
int contadorSolesAleatorios = 0;

// Actualizamos el estado del juego en cada turno
void actualizarTurno(
    int **jardin,
    Zombie *zombies,
    int *cantidadZombies,
    int *soles,
    int *contadorGirasoles,
    int *juegoTerminado,
    int *cortacesped,
    int *filas,
    int *columnas,
    int **contadoresAtaque,
    Sol *solesGenerados,
    int *cantidadSoles,
    int *capacidadSoles
){

    // Contamos la cantidad de girasoles que hay en el jardin
    int cantidadGirasoles = 0;

    for(int i = 0; i < *filas; i++){
        for(int j = 0; j < *columnas; j++){
            if(jardin[i][j] == CELDA_GIRASOL){
                cantidadGirasoles++;
            }
        }
    }

    // Aumentamos el contador de turnos para generar soles
    (*contadorGirasoles)++;

    if(*contadorGirasoles >= 2 && cantidadGirasoles > 0){

        // Contamos los soles que realmente se generan
        int solesGirasolesGenerados = 0;

        // Generamos un sol por cada girasol
        for(int i = 0; i < *filas; i++){
            for(int j = 0; j < *columnas; j++){

                if(jardin[i][j] == CELDA_GIRASOL &&
                   *cantidadSoles < *capacidadSoles){

                    // Guardamos la posicion y el valor del sol
                    solesGenerados[*cantidadSoles].x = j * 80 + 40;
                    solesGenerados[*cantidadSoles].y = i * 80 + 40;
                    solesGenerados[*cantidadSoles].valor = 50;
                    solesGenerados[*cantidadSoles].activo = 1;

                    // Aumentamos la cantidad de soles generados
                    (*cantidadSoles)++;
                    solesGirasolesGenerados++;
                }
            }
        }

        // Mostramos solamente los soles que se generaron
        if(solesGirasolesGenerados > 0){
            printf("Los girasoles generaron %d soles\n",
                   50 * solesGirasolesGenerados);
        }

        // Reiniciamos el contador de girasoles
        *contadorGirasoles = 0;
    }

    // Generamos soles aleatorios cada 5 turnos
    contadorSolesAleatorios++;

    // Comprobamos si corresponde generar un sol aleatorio
    if(contadorSolesAleatorios >= 5 &&
       *cantidadSoles < *capacidadSoles){

        // Buscamos una posicion vacia en el jardin
        for(int intento = 0; intento < 20; intento++){

            int filaSol = rand() % *filas;
            int columnaSol = rand() % *columnas;

            if(jardin[filaSol][columnaSol] == CELDA_VACIA){

                // Creamos el sol en la posicion encontrada
                solesGenerados[*cantidadSoles].x = columnaSol * 80 + 40;
                solesGenerados[*cantidadSoles].y = filaSol * 80 + 40;
                solesGenerados[*cantidadSoles].valor = 50;
                solesGenerados[*cantidadSoles].activo = 1;

                // Aumentamos la cantidad de soles generados
                (*cantidadSoles)++;

                printf("Aparecio un sol aleatorio\n");

                break;
            }
        }

        // Reiniciamos el contador despues del intento
        contadorSolesAleatorios = 0;
    }

    // Recorremos las plantas que pueden atacar a los zombies
    for(int i = 0; i < *filas; i++){
        for(int j = 0; j < *columnas; j++){

            int dano = 0;
            float frecuencia = 0;

            // Identificamos el tipo de planta y sus caracteristicas
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

            // Verificamos si la casilla contiene una planta que ataca
            if(dano > 0){

                // Aumentamos el contador de recarga de la planta
                contadoresAtaque[i][j]++;

                if(contadoresAtaque[i][j] >= frecuencia){

                    int zombieCercano = -1;
                    int columnaCercana = *columnas;

                    // El gato espinoso ataca a todos los zombies de su fila
                    if(jardin[i][j] == CELDA_GATOESPINOSO){

                        for(int k = 0; k < *cantidadZombies; k++){

                            // Comprobamos que el zombie este activo y delante de la planta
                            if(zombies[k].activo == 1 &&
                               zombies[k].fila == i &&
                               zombies[k].columna > j){

                                // Aplicamos el dano al zombie
                                zombies[k].vida -= dano;

                                printf("El gato espinoso ataco al zombie %d. Vida: %d\n",
                                       k + 1, zombies[k].vida);

                                // Comprobamos si el zombie fue derrotado
                                if(zombies[k].vida <= 0){

                                    jardin[zombies[k].fila][zombies[k].columna] = CELDA_VACIA;
                                    zombies[k].activo = 0;

                                    printf("Zombie %d fue derrotado\n", k + 1);
                                }
                            }
                        }
                    }
                    else{

                        // Buscamos el zombie activo mas cercano de la misma fila
                        for(int k = 0; k < *cantidadZombies; k++){

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

                            // Aplicamos el dano al zombie
                            zombies[zombieCercano].vida -= dano;

                            // Mostramos el tipo de planta que realizo el ataque
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

                            // Comprobamos si el zombie fue derrotado
                            if(zombies[zombieCercano].vida <= 0){

                                jardin[zombies[zombieCercano].fila][zombies[zombieCercano].columna] = CELDA_VACIA;
                                zombies[zombieCercano].activo = 0;

                                printf("Zombie %d fue derrotado\n", zombieCercano + 1);
                            }
                        }
                    }

                    // Reiniciamos el contador de ataque de la planta
                    contadoresAtaque[i][j] = 0;
                }
            }
        }
    }

    // Recorremos todos los zombies de la partida
    for(int i = 0; i < *cantidadZombies; i++){

        // Verificamos si el zombie sigue activo
        if(zombies[i].activo == 1){

            // Comprobamos si el zombie fue derrotado
            if(zombies[i].vida <= 0){

                jardin[zombies[i].fila][zombies[i].columna] = CELDA_VACIA;
                zombies[i].activo = 0;

                printf("Zombie %d fue derrotado\n", i + 1);
            }
            else{

                // Verificamos si el zombie llego a la casa
                if(zombies[i].columna == 0){

                    // Si el cortacesped ya fue utilizado, el jugador pierde
                    if(cortacesped[zombies[i].fila] == 0){

                        printf("Un zombie llego a la casa. Perdiste :(\n");
                        *juegoTerminado = 1;
                        return;
                    }

                    // Activamos el cortacesped de la fila correspondiente
                    activarCortacesped(
                        jardin,
                        zombies[i].fila,
                        &cortacesped[zombies[i].fila],
                        *columnas
                    );

                    // Eliminamos todos los zombies de esa fila
                    for(int j = 0; j < *cantidadZombies; j++){

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
