#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "jardin.h"
#include "zombies.h"
#include "estructuras.h"

// Crea una matriz dinamica para representar el jardin.
int **crearJardin(int *filas, int *columnas){

    if(filas == NULL || columnas == NULL ||
       *filas <= 0 || *columnas <= 0){
        return NULL;
    }

    int **jardin = malloc(*filas * sizeof(int *));

    if(jardin == NULL){
        return NULL;
    }

    for(int i = 0; i < *filas; i++){

        jardin[i] = malloc(*columnas * sizeof(int));

        if(jardin[i] == NULL){

            // Liberamos las filas que ya fueron reservadas.
            for(int j = 0; j < i; j++){
                free(jardin[j]);
            }

            free(jardin);
            return NULL;
        }

        // Inicializamos todas las casillas vacias.
        for(int j = 0; j < *columnas; j++){
            jardin[i][j] = 0;
        }
    }

    return jardin;
}


// Lee las dimensiones y el contenido inicial del jardin desde un archivo.
int **leerJardin(
    const char *nombreArchivo,
    int *filas,
    int *columnas
){

    if(nombreArchivo == NULL || filas == NULL || columnas == NULL){
        return NULL;
    }

    // Inicializamos las salidas.
    *filas = 0;
    *columnas = 0;

    FILE *archivo = fopen(nombreArchivo, "r");

    // Comprobamos que el archivo se haya abierto correctamente.
    if(archivo == NULL){
        printf("Error: no se pudo abrir el archivo %s.\n", nombreArchivo);
        return NULL;
    }

    if(fscanf(archivo, "%d %d", filas, columnas) != 2 ||
       *filas <= 0 || *columnas <= 0){

        printf("Error: dimensiones del jardin invalidas.\n");
        fclose(archivo);
        return NULL;
    }

    int **jardin = crearJardin(filas, columnas);

    if(jardin == NULL){
        printf("Error: no se pudo reservar memoria para el jardin.\n");
        fclose(archivo);
        *filas = 0;
        *columnas = 0;
        return NULL;
    }

    for(int i = 0; i < *filas; i++){
        for(int j = 0; j < *columnas; j++){

            if(fscanf(archivo, "%d", &jardin[i][j]) != 1){

                printf("Error: datos incompletos en el archivo del jardin.\n");

                liberarJardin(jardin, filas);
                *filas = 0;
                *columnas = 0;

                fclose(archivo);
                return NULL;
            }

            // El archivo inicial solo admite casillas vacias o plantas.
            if(jardin[i][j] < 0 || jardin[i][j] > CELDA_NUEZREFORZADA ||
               jardin[i][j] == CELDA_ZOMBIE){

                printf("Error: valor de casilla invalido en el jardin.\n");

                liberarJardin(jardin, filas);
                *filas = 0;
                *columnas = 0;

                fclose(archivo);
                return NULL;
            }
        }
    }

    // Comprobamos que no existan datos adicionales.
    int datoExtra;

    if(fscanf(archivo, "%d", &datoExtra) == 1){

        printf("Error: existen datos adicionales en el archivo del jardin.\n");

        liberarJardin(jardin, filas);
        *filas = 0;
        *columnas = 0;

        fclose(archivo);
        return NULL;
    }

    fclose(archivo);
    return jardin;
}


// Imprime el contenido de la matriz en consola.
void imprimirMatriz(int **jardin, int *filas, int *columnas){

    if(jardin == NULL || filas == NULL || columnas == NULL){
        return;
    }

    for(int i = 0; i < *filas; i++){

        for(int j = 0; j < *columnas; j++){
            printf("%d ", jardin[i][j]);
        }

        printf("\n");
    }
}


// Dibuja el jardin y los zombies en pantalla.
void dibujarJardin(
    int **jardin,
    int *filas,
    int *columnas,
    Zombie zombies[],
    int *cantidadZombies
){

    if(jardin == NULL || filas == NULL ||
       columnas == NULL || cantidadZombies == NULL){
        return;
    }

    int anchoCelda = 80;
    int altoCelda = 80;

    for(int i = 0; i < *filas; i++){

        for(int j = 0; j < *columnas; j++){

            int x = j * anchoCelda;
            int y = i * altoCelda;

            DrawRectangle(x, y, anchoCelda, altoCelda, GREEN);
            DrawRectangleLines(x, y, anchoCelda, altoCelda, DARKGREEN);

            switch(jardin[i][j]){

                case CELDA_LANZAGUISANTES:
                    DrawCircle(
                        x + anchoCelda / 2,
                        y + altoCelda / 2,
                        20,
                        LIME
                    );
                    break;

                case CELDA_GIRASOL:
                    DrawCircle(
                        x + anchoCelda / 2,
                        y + altoCelda / 2,
                        20,
                        YELLOW
                    );
                    break;

                case CELDA_NUEZ:
                    DrawRectangle(
                        x + 15,
                        y + 15,
                        50,
                        50,
                        BROWN
                    );
                    break;

                case CELDA_GATOESPINOSO:
                    // Dibujamos el gato espinoso.
                    DrawCircle(
                        x + anchoCelda / 2,
                        y + altoCelda / 2,
                        22,
                        PINK
                    );
                    DrawTriangle(
                        (Vector2){x + 25, y + 30},
                        (Vector2){x + 30, y + 10},
                        (Vector2){x + 42, y + 27},
                        PINK
                    );
                    DrawTriangle(
                        (Vector2){x + 50, y + 27},
                        (Vector2){x + 62, y + 10},
                        (Vector2){x + 65, y + 30},
                        PINK
                    );
                    break;

                case CELDA_LANZAGUISANTESFUEGO:
                    // Dibujamos el lanzaguisantes de fuego.
                    DrawCircle(
                        x + anchoCelda / 2,
                        y + altoCelda / 2,
                        23,
                        ORANGE
                    );
                    DrawCircle(
                        x + anchoCelda / 2,
                        y + altoCelda / 2,
                        12,
                        RED
                    );
                    break;

                case CELDA_NUEZREFORZADA:
                    // Dibujamos la nuez reforzada.
                    DrawRectangle(
                        x + 10,
                        y + 10,
                        60,
                        60,
                        DARKBROWN
                    );
                    DrawRectangleLines(
                        x + 10,
                        y + 10,
                        60,
                        60,
                        BLACK
                    );
                    break;
            }
        }
    }

    // Dibujamos los zombies activos.
    for(int i = 0; i < *cantidadZombies; i++){

        if(zombies[i].activo == 1){

            int x = zombies[i].columna * anchoCelda;
            int y = zombies[i].fila * altoCelda;

            if(zombies[i].tipoZombie == ZOMBIE_NORMAL){

                // Dibujamos el zombie normal.
                DrawCircle(
                    x + anchoCelda / 2,
                    y + altoCelda / 2,
                    25,
                    RED
                );
            }
            else if(zombies[i].tipoZombie == ZOMBIE_BANDERA){

                // Dibujamos el zombie abanderado.
                DrawCircle(
                    x + anchoCelda / 2,
                    y + altoCelda / 2,
                    25,
                    BLUE
                );
            }
            else if(zombies[i].tipoZombie == ZOMBIE_CONO){
                // Dibujamos el zombie con cono.
                DrawCircle(
                    x + anchoCelda / 2,
                    y + altoCelda / 2,
                    25,
                    GRAY
                );
            }
            else if(zombies[i].tipoZombie == ZOMBIE_CUBETA){

                // Dibujamos el zombie con cubeta.
                DrawCircle(
                    x + anchoCelda / 2,
                    y + altoCelda / 2,
                    25,
                    DARKGREEN
                );

                DrawRectangle(
                    x + 25,
                    y + 8,
                    30,
                    25,
                    GRAY
                );

                DrawRectangleLines(
                    x + 25,
                    y + 8,
                    30,
                    25,
                    DARKGRAY
                );
            }
        }
    }
}


// Libera la memoria dinamica del jardin.
void liberarJardin(int **jardin, int *filas){

    if(jardin == NULL || filas == NULL){
        return;
    }

    for(int i = 0; i < *filas; i++){
        free(jardin[i]);
    }

    free(jardin);
}
