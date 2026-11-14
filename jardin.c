
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "jardin.h"
#include "zombies.h"

// creamos la matriz dinamica
int **crearJardin(int filas, int columnas){

    int **jardin=malloc(filas*sizeof(int *));

    for(int i=0; i<filas; i++){
        jardin[i]=malloc(columnas*sizeof(int));
    }

    // rellenamos la matriz con 0
    for(int i=0; i<filas; i++){
        for(int j=0; j<columnas; j++){
            jardin[i][j]=0;
        }
    }

    return jardin;
}

int leerJardin(const char *nombreArchivo, int ***jardin, int *filas, int *columnas) {

    FILE *archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL) {
        return 0;
    }

    *jardin = NULL;
    *filas = 0;
    *columnas = 0;

    // Leemos las dimensiones del jardin
    if (fscanf(archivo, "%d %d", filas, columnas) != 2 ||
        *filas <= 0 || *columnas <= 0) {

        fclose(archivo);
        return 0;
    }

    // Creamos la matriz dinamicamente
    int **matriz = crearJardin(*filas, *columnas);

    if (matriz == NULL) {
        fclose(archivo);
        *filas = 0;
        *columnas = 0;
        return 0;
    }

    // Leemos los valores de cada casilla
    for (int i = 0; i < *filas; i++) {
        for (int j = 0; j < *columnas; j++) {

            if (fscanf(archivo, "%d", &matriz[i][j]) != 1) {
                liberarJardin(matriz, *filas);
                fclose(archivo);

                *jardin = NULL;
                *filas = 0;
                *columnas = 0;

                return 0;
            }

            // Validamos los valores permitidos
            if (matriz[i][j] < 0 || matriz[i][j] > 3) {
                liberarJardin(matriz, *filas);
                fclose(archivo);

                *jardin = NULL;
                *filas = 0;
                *columnas = 0;

                return 0;
            }
        }
    }

    // Comprobamos que no existan datos adicionales
    char extra;

    if (fscanf(archivo, " %c", &extra) == 1) {
        liberarJardin(matriz, *filas);
        fclose(archivo);

        *jardin = NULL;
        *filas = 0;
        *columnas = 0;

        return 0;
    }

    fclose(archivo);

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

    for(int i=0; i<filas; i++){
        free(jardin[i]);
    }

    free(jardin);
}
