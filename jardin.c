#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <raylib.h>

#include "jardin.h"
#include "zombies.h"

// creamos la matriz dinamica
int **crearJardin(int filas, int columnas){

    if(filas <= 0 || columnas <= 0){
        return NULL;
    }

    // verificamos que el tamaño de memoria no desborde
    if((size_t)filas > SIZE_MAX / sizeof(int *) ||
       (size_t)columnas > SIZE_MAX / sizeof(int)){
        return NULL;
    }

    int **jardin = malloc((size_t)filas * sizeof(int *));

    if(jardin == NULL){
        return NULL;
    }

    for(int i = 0; i < filas; i++){

        jardin[i] = malloc((size_t)columnas * sizeof(int));

        if(jardin[i] == NULL){

            for(int j = 0; j < i; j++){
                free(jardin[j]);
            }

            free(jardin);
            return NULL;
        }
    }

    // rellenamos la matriz con 0
    for(int i = 0; i < filas; i++){
        for(int j = 0; j < columnas; j++){
            jardin[i][j] = 0;
        }
    }

    return jardin;
}

// leemos el jardin desde un archivo
int leerJardin(const char *nombreArchivo, int ***jardin, int *filas, int *columnas){

    if(nombreArchivo == NULL || jardin == NULL || filas == NULL || columnas == NULL){
        return 0;
    }

    // inicializamos las variables de salida
    *jardin = NULL;
    *filas = 0;
    *columnas = 0;

    FILE *archivo = fopen(nombreArchivo, "r");

    if(archivo == NULL){
        printf("Error: no se pudo abrir el archivo del jardin.\n");
        return 0;
    }

    // leemos las dimensiones del jardin
    if(fscanf(archivo, "%d %d", filas, columnas) != 2 || *filas <= 0 || *columnas <= 0){

        printf("Error: dimensiones del jardin invalidas.\n");

        fclose(archivo);

        *filas = 0;
        *columnas = 0;

        return 0;
    }

    // creamos la matriz dinamica
    int **matriz = crearJardin(*filas, *columnas);

    if(matriz == NULL){

        printf("Error: no se pudo reservar memoria para el jardin.\n");

        fclose(archivo);

        *filas = 0;
        *columnas = 0;

        return 0;
    }

    // leemos los elementos de la matriz
    for(int i = 0; i < *filas; i++){
        for(int j = 0; j < *columnas; j++){

            if(fscanf(archivo, "%d", &matriz[i][j]) != 1){

                printf("Error: faltan datos en el archivo del jardin.\n");

                liberarJardin(matriz, *filas);
                fclose(archivo);

                *filas = 0;
                *columnas = 0;

                return 0;
            }

            // verificamos que el elemento sea valido
            if(matriz[i][j] < 0 || matriz[i][j] > 3){

                printf("Error: elemento no valido en el jardin.\n");

                liberarJardin(matriz, *filas);
                fclose(archivo);

                *filas = 0;
                *columnas = 0;

                return 0;
            }
        }
    }

    // verificamos que no existan datos adicionales
    int caracter;

    while((caracter = fgetc(archivo)) != EOF){

        if(!isspace((unsigned char)caracter)){

            printf("Error: existen datos adicionales en el archivo del jardin.\n");

            liberarJardin(matriz, *filas);
            fclose(archivo);

            *filas = 0;
            *columnas = 0;

            return 0;
        }
    }

    fclose(archivo);

    // entregamos la matriz creada
    *jardin = matriz;

    return 1;
}

// imprimimos la matriz
void imprimirMatriz(int **jardin, int filas, int columnas){

    for(int i=0; i<filas; i++){
        for(int j=0; j<columnas; j++){
            printf("%d ", jardin[i][j]);
        }
        printf("\n");
    }
}

// dibujamos el jardin
void dibujarJardin(int **jardin, int filas, int columnas, Zombie zombies[], int cantidadZombies){

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("PLANTS VS ZOMBIES", 250, 20, 30, DARKGREEN);

    // dibujamos el jardin
    for(int i=0; i<filas; i++){
        for(int j=0; j<columnas; j++){

            int x=100+j*70;
            int y=100+i*70;

            DrawRectangle(x, y, 70, 70, LIME);
            DrawRectangleLines(x, y, 70, 70, DARKGREEN);

            if(jardin[i][j]==1){
                DrawCircle(x+35, y+35, 20, BLUE);
            }
            else if(jardin[i][j]==2){
                DrawCircle(x+35, y+35, 20, YELLOW);
            }
            else if(jardin[i][j]==3){
                DrawCircle(x+35, y+35, 20, BROWN);
            }
        }
    }

    // dibujamos los zombies segun su tipo
    for(int i=0; i<cantidadZombies; i++){

        if(zombies[i].activo==1){

            int x=100+zombies[i].columna*70;
            int y=100+zombies[i].fila*70;

            // zombie normal
            if(zombies[i].tipoZombie==0){
                DrawCircle(x+35, y+35, 20, RED);
            }
            // zombie abanderado
            else{
                DrawCircle(x+35, y+35, 20, ORANGE);

                DrawLine(x+35, y+15, x+35, y-5, BLACK);

                DrawTriangle(
                    (Vector2){x+35, y-5},
                    (Vector2){x+55, y+2},
                    (Vector2){x+35, y+10},
                    RED
                );
            }
        }
    }

    EndDrawing();
}

// liberamos la memoria
void liberarJardin(int **jardin, int filas){

    if(jardin == NULL){
        return;
    }

    for(int i=0; i<filas; i++){
        free(jardin[i]);
    }

    free(jardin);
}
