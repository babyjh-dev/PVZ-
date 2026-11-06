#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "jardin.h"//algo no definido, que yo cree

//creamos la matriz dinamica
int**crearJardin(int filas, int columnas){

    int**jardin= malloc(filas*sizeof(int *));

    for(int i=0; i<filas;i++){
        jardin[i]=malloc(columnas * sizeof(int));
    }

     // rellenamos la matriz con 0
    for(int i=0; i<filas; i++){
        for(int j=0; j<columnas; j++){
            jardin[i][j]=0;
        }
    }
     return jardin;
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
//dibujamos el jardin
void dibujarJardin(int **jardin, int filas, int columnas){

    // comenzamos a dibujar
    BeginDrawing();
    // definimos el fondo de la ventana
    ClearBackground(RAYWHITE);

    // escribimos el titulo del juego
    DrawText("PLANTS VS ZOMBIES", 250, 20, 30, DARKGREEN);

    // recorremos la matriz para dibujar el jardin
    for(int i=0; i<filas; i++){
        for(int j=0; j<columnas; j++){

            // definimos la posicion de cada casilla
            int x=100+j*70;
            int y=100+i*70;

            // dibujamos las casillas del jardin
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
            else if(jardin[i][j]==4){
                DrawCircle(x+35, y+35, 20, RED);
            }
        }
    }

    // terminamos de dibujar
    EndDrawing();
}
//liberamos la memoria
void liberarJardin(int**jardin, int filas){

    for(int i=0; i<filas; i++){
        free(jardin[i]);
    }

    free(jardin);
}