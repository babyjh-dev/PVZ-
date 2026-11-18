#include <stdio.h>
#include <stdlib.h>
#include "zombies.h"

// colocamos el zombie en el jardin
void colocarZombie(int **jardin, int fila, int columna){
    jardin[fila][columna] = CELDA_ZOMBIE;
}

// inicializamos los datos de un zombie
void inicializarZombie(Zombie *zombie, int fila, int columna, ZombieConfig configuracion){
    zombie->fila = fila;
    zombie->columna = columna;
    zombie->vida = configuracion.vida;
    zombie->activo = 1;
    zombie->contadorAtaque = 0;
    zombie->contadorComida = 0;
    zombie->contadorMovimiento = 0;

    // elegimos aleatoriamente el tipo
    zombie->tipoZombie = configuracion.tipo;
    zombie->dano = configuracion.dano;
    zombie->velocidad = configuracion.velocidad;
    zombie->frecuenciaAtaque = configuracion.frecuenciaAtaque;
}

// generamos los zombies de una oleada
void generarOleadas(
    int **jardin,
    Zombie zombies[],
    int *cantidadZombies,
    int *zombiesGenerados,
    int zombiesPorOleada,
    int capacidadZombies,
    int filas,
    int columnas,
    ZombieConfig configuraciones[],
    int cantidadTipos
) {
    // revisamos si faltan zombies por generar
    if (*zombiesGenerados >= zombiesPorOleada) return;
    if (*cantidadZombies >= capacidadZombies) return;
    if (cantidadTipos <= 0) return;

    int columna = columnas - 1;
    int inicio = rand() % filas;

    // buscamos una casilla vacia en la ultima columna
    for (int j = 0; j < filas; j++) {
        int fila = (inicio + j) % filas;

        if (jardin[fila][columna] == CELDA_VACIA) {
            colocarZombie(jardin, fila, columna);

            int tipoElegido = *zombiesGenerados % cantidadTipos;
            printf("Zombie generado: %d\n", tipoElegido);

            inicializarZombie(
                &zombies[*cantidadZombies],
                fila,
                columna,
                configuraciones[tipoElegido]
            );

            (*cantidadZombies)++;
            (*zombiesGenerados)++;
            break;
        }
    }
}

// movemos el zombie
void moverZombie(int **jardin, Zombie *zombie){

    // definimos la velocidad del zombie
    int velocidad = 2;

    // el abanderado avanza mas rapido
    if(zombie->tipoZombie == ZOMBIE_BANDERA) velocidad = 1;

    zombie->contadorMovimiento++;

    if(zombie->contadorMovimiento < velocidad) return;

    zombie->contadorMovimiento = 0;

    // verificamos que el zombie este activo y pueda avanzar
    if(zombie->activo == 1 && zombie->columna > 0){

        int siguienteColumna = zombie->columna - 1;

        // verificamos si hay una planta
        if(jardin[zombie->fila][siguienteColumna] == CELDA_LANZAGUISANTES ||
           jardin[zombie->fila][siguienteColumna] == CELDA_GIRASOL ||
           jardin[zombie->fila][siguienteColumna] == CELDA_NUEZ ||
           jardin[zombie->fila][siguienteColumna] == CELDA_GATOESPINOSO ||
           jardin[zombie->fila][siguienteColumna] == CELDA_LANZAGUISANTESFUEGO ||
           jardin[zombie->fila][siguienteColumna] == CELDA_NUEZREFORZADA){

            zombie->contadorComida++;

            printf("el zombie se esta comiendo una planta\n");

            int resistencia = 2;

            if(jardin[zombie->fila][siguienteColumna] == CELDA_NUEZ){
                resistencia = 5;
            }

            if(jardin[zombie->fila][siguienteColumna] == CELDA_NUEZREFORZADA){
                resistencia = 10;
            }

            if(zombie->contadorComida >= resistencia){

                jardin[zombie->fila][siguienteColumna] = CELDA_VACIA;

                printf("el zombie destruyo la planta\n");

                zombie->contadorComida = 0;
            }
        }
        else if(jardin[zombie->fila][siguienteColumna] == CELDA_ZOMBIE){

            // el abanderado puede saltar sobre otro zombie
            if(zombie->tipoZombie == ZOMBIE_BANDERA &&
               siguienteColumna > 0 &&
               jardin[zombie->fila][siguienteColumna - 1] == CELDA_VACIA){

                jardin[zombie->fila][zombie->columna] = CELDA_VACIA;

                zombie->columna -= 2;

                jardin[zombie->fila][zombie->columna] = CELDA_ZOMBIE;

                zombie->contadorComida = 0;

            } else {

                // si no puede saltar, espera
                return;
            }
        }
        else {

            jardin[zombie->fila][zombie->columna] = CELDA_VACIA;

            zombie->columna--;

            jardin[zombie->fila][zombie->columna] = CELDA_ZOMBIE;

            zombie->contadorComida = 0;
        }
    }
}

void activarCortacesped(int **jardin, int fila, int *cortacesped, int columnas){

    // verificamos si el cortacesped esta disponible
    if(*cortacesped == 1){

        printf("el cortacesped de la fila %d se activo \n", fila + 1);

        // eliminamos los zombies de esa fila
        for (int j = 0; j < columnas; j++){
            if(jardin[fila][j] == CELDA_ZOMBIE){
                jardin[fila][j] = CELDA_VACIA;
            }
        }

        *cortacesped = 0;
    }
}
