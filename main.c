#include <stdio.h>
#include <stdlib.h> //para usar malloc y free
#include <raylib.h> //para la visual

#include "jardin.h"
#include "plantas.h"
#include "zombies.h"
#include "juego.h"


int main(){

    // abrimos la ventana del juego
    InitWindow(900, 600, "Plants vs Zombies");
    SetTargetFPS(60);

    int filas=5;
    int columnas=9;

    //creamos el jardin
    int **jardin=crearJardin(filas, columnas);

    int soles=200;
    printf("El juego comenzo\n");
    int girasolComprado=0;

    int filaPlanta=-1;
    int columnaPlanta=-1;

    //preguntamos si queremos seguir comprando plantas
    int seguirComprando=1;

    while(seguirComprando==1){
    printf("Entrando a la tienda\n");
        //compramos y colocamos una planta 
    comprarPlanta(jardin, filas, columnas, &soles,&girasolComprado, &filaPlanta, &columnaPlanta);
        //preguntamos si queremos comprar otrea planta
        printf("\nQuieres comprar otra planta? (1:SI/0:NO): ");
        scanf("%d", &seguirComprando);
    }

    int fila;

    // pedimos la fila donde aparecera el zombie
    printf("Ingrese la fila donde aparecera el zombie (0 a 4): ");
    scanf("%d", &fila);

    // definimos la posicion del zombie
    int columnaZombie=8;

    // colocamos el zombie en la matriz
    colocarZombie(jardin, fila, columnaZombie);


    int vidaZombie=3;
    int turno=1;
    int contadorAtaque=0;
    int contadorGirasoles=0;

    // definimos si el juego termino
    int juegoTerminado=0;

    // guardamos el tiempo inicial
    float tiempoAnterior=GetTime();

    // mantenemos la ventana abierta
    while(!WindowShouldClose()){

        // verificamos si paso un segundo
        if(GetTime()-tiempoAnterior>=1.0f && juegoTerminado==0){

            printf("\nTurno %d:\n", turno);

            //actualizamos el turno del juego
            actualizarTurno(jardin, fila, filaPlanta, &columnaZombie, columnaPlanta, &vidaZombie, &contadorAtaque, &soles, girasolComprado, &contadorGirasoles, &juegoTerminado);

            // imprimimos la matriz actualizada
            imprimirMatriz(jardin, filas, columnas);

            // actualizamos el tiempo y el turno
            tiempoAnterior=GetTime();
            turno++;
        }
        //dibujamos el jardin en la ventana
        dibujarJardin(jardin, filas, columnas);
    }

    // cerramos la ventana
    CloseWindow();

    // liberamos la memoria de la matriz
    liberarJardin(jardin, filas);

    return 0;
}
