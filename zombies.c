#include <stdio.h>
#include <stdlib.h>
#include "zombies.h"


// Colocamos el zombie en el jardin
void colocarZombie(int **jardin, int fila, int columna){
    jardin[fila][columna] = CELDA_ZOMBIE;
}


// Inicializamos los datos de un zombie
void inicializarZombie(Zombie *zombie, int fila, int columna, ZombieConfig *configuracion){

    // Asignamos la posicion inicial del zombie
    zombie->fila = fila;
    zombie->columna = columna;

    // Asignamos sus caracteristicas iniciales
    zombie->vida = configuracion->vida;
    zombie->activo = 1;
    zombie->contadorAtaque = 0;
    zombie->contadorComida = 0;
    zombie->contadorMovimiento = 0;

    // Asignamos el tipo y las caracteristicas de la configuracion
    zombie->tipoZombie = configuracion->tipo;
    zombie->dano = configuracion->dano;
    zombie->velocidad = configuracion->velocidad;
    zombie->frecuenciaAtaque = configuracion->frecuenciaAtaque;
}


// Generamos los zombies de una oleada
void generarOleadas(
    int **jardin,
    Zombie *zombies,
    int *cantidadZombies,
    int *zombiesGenerados,
    int *zombiesPorOleada,
    int *capacidadZombies,
    int *filas,
    int *columnas,
    ZombieConfig *configuraciones,
    int *cantidadTipos
) {

    // Revisamos si faltan zombies por generar y si hay espacio disponible
    if (*zombiesGenerados >= *zombiesPorOleada) return;
    if (*cantidadZombies >= *capacidadZombies) return;
    if (*cantidadTipos <= 0) return;

    // Definimos la ultima columna y elegimos una fila aleatoria para comenzar
    int columna = *columnas - 1;
    int inicio = rand() % *filas;

    // Buscamos una casilla vacia en la ultima columna
    for (int j = 0; j < *filas; j++) {
        int fila = (inicio + j) % *filas;

        if (jardin[fila][columna] == CELDA_VACIA) {

            // Colocamos el zombie en la casilla encontrada
            colocarZombie(jardin, fila, columna);

            // Elegimos el tipo de zombie que se va a generar
            int tipoElegido = *zombiesGenerados % *cantidadTipos;
            printf("Zombie generado: %d\n", tipoElegido);

            // Inicializamos los datos del zombie
            inicializarZombie(
                &zombies[*cantidadZombies],
                fila,
                columna,
                &configuraciones[tipoElegido]
            );

            // Actualizamos las cantidades de zombies
            (*cantidadZombies)++;
            (*zombiesGenerados)++;

            break;
        }
    }
}


// Movemos el zombie por el jardin
void moverZombie(int **jardin, Zombie *zombie){

    // Obtenemos la velocidad del zombie desde su configuracion
    int velocidad = zombie->velocidad;

    // Aumentamos el contador de movimiento
    zombie->contadorMovimiento++;

    // Esperamos hasta que corresponda mover al zombie
    if(zombie->contadorMovimiento < velocidad) return;

    zombie->contadorMovimiento = 0;

    // Verificamos que el zombie este activo y pueda avanzar
    if(zombie->activo == 1 && zombie->columna > 0){

        // Calculamos la siguiente columna
        int siguienteColumna = zombie->columna - 1;

        // Verificamos si hay una planta en la siguiente casilla
        if(jardin[zombie->fila][siguienteColumna] == CELDA_LANZAGUISANTES ||
           jardin[zombie->fila][siguienteColumna] == CELDA_GIRASOL ||
           jardin[zombie->fila][siguienteColumna] == CELDA_NUEZ ||
           jardin[zombie->fila][siguienteColumna] == CELDA_GATOESPINOSO ||
           jardin[zombie->fila][siguienteColumna] == CELDA_LANZAGUISANTESFUEGO ||
           jardin[zombie->fila][siguienteColumna] == CELDA_NUEZREFORZADA){

            // Aumentamos el contador de veces que el zombie come la planta
            zombie->contadorComida++;

            printf("el zombie se esta comiendo una planta\n");

            // Definimos cuantas veces debe atacar para destruir la planta
            int resistencia = 2;

            // La nuez necesita mas ataques para ser destruida
            if(jardin[zombie->fila][siguienteColumna] == CELDA_NUEZ){
                resistencia = 5;
            }

            // La nuez reforzada tiene una mayor resistencia
            if(jardin[zombie->fila][siguienteColumna] == CELDA_NUEZREFORZADA){
                resistencia = 10;
            }

            // Comprobamos si el zombie ya destruyo la planta
            if(zombie->contadorComida >= resistencia){

                jardin[zombie->fila][siguienteColumna] = CELDA_VACIA;

                printf("el zombie destruyo la planta\n");

                // Reiniciamos el contador de comida
                zombie->contadorComida = 0;
            }
        }
        else if(jardin[zombie->fila][siguienteColumna] == CELDA_ZOMBIE){

            // El abanderado puede saltar sobre otro zombie
            if(zombie->tipoZombie == ZOMBIE_BANDERA &&
               siguienteColumna > 0 &&
               jardin[zombie->fila][siguienteColumna - 1] == CELDA_VACIA){

                // Quitamos al zombie de su posicion anterior
                jardin[zombie->fila][zombie->columna] = CELDA_VACIA;

                // Avanzamos dos columnas
                zombie->columna -= 2;

                // Actualizamos la posicion del zombie en el jardin
                jardin[zombie->fila][zombie->columna] = CELDA_ZOMBIE;

                zombie->contadorComida = 0;

            } else {

                // Si no puede saltar, el zombie espera
                return;
            }
        }
        else {

            // Quitamos al zombie de su posicion anterior
            jardin[zombie->fila][zombie->columna] = CELDA_VACIA;

            // Avanzamos una columna hacia la izquierda
            zombie->columna--;

            // Actualizamos su nueva posicion en el jardin
            jardin[zombie->fila][zombie->columna] = CELDA_ZOMBIE;

            // Reiniciamos el contador de comida
            zombie->contadorComida = 0;
        }
    }
}


// Activamos el cortacesped de una fila
void activarCortacesped(int **jardin, int fila, int *cortacesped, int columnas){

    // Verificamos si el cortacesped de esa fila esta disponible
    if(*cortacesped == 1){

        printf("el cortacesped de la fila %d se activo \n", fila + 1);

        // Eliminamos los zombies que se encuentren en esa fila
        for (int j = 0; j < columnas; j++){
            if(jardin[fila][j] == CELDA_ZOMBIE){
                jardin[fila][j] = CELDA_VACIA;
            }
        }

        // Indicamos que el cortacesped ya fue utilizado
        *cortacesped = 0;
    }
}
