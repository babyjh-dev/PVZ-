#include <stdio.h>
#include <stdlib.h> //para usar malloc y free
#include <raylib.h> //para la visual
#include <time.h> //para usar time

#include "jardin.h"
#include "plantas.h"
#include "zombies.h"
#include "juego.h"


int main(){
    srand(time(NULL));//iniciamos los numeros aleatorios

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
        //preguntamos si queremos comprar otra planta
        printf("\nQuieres comprar otra planta? (1:SI/0:NO): ");
        scanf("%d", &seguirComprando);
    }
    //guardamos los datos de los zombies
    Zombie zombies[MAX_ZOMBIES];
    printf("sali de la tienda correctamente\n");

    int cantidadZombies=rand()%3+1;
        printf("Cantidad de zombies: %d\n", cantidadZombies);
    //creamos un cortacesped para cada fila y cuando se utilice cambia a 0
    int cortacesped[5]={1,1,1,1,1};

    //colocamos los zombies en el jardin
    for(int i =0; i<cantidadZombies; i++){
        //elegimos una fila aleatoria
        zombies[i].fila=rand()%filas;
        printf("Fila elegida: %d\n", zombies[i].fila);
        printf("Valor de la casilla: %d\n", jardin[zombies[i].fila][8]);

        //verificamos si la fila no esta ocupadda
        while(jardin[zombies[i].fila][8]!=0){
            printf("Fila ocupada, buscando otra...\n");
            zombies[i].fila=rand()%filas;
        }
        printf("Fila disponible encontrada\n");

        //definimos la posicion inicial
        zombies[i].columna=8;

        //definimos la vida del zombie
        zombies[i].vida=3;

        //indicamos que esta activo
        zombies[i].activo=1;

        //iniciamos el contador de ataque
        zombies[i].contadorAtaque=0;

        //iniciamos el contador de comer plantas
        zombies[i].contadorComida=0;

        zombies[i].contadorMovimiento=0;
        //elegimos aleatoriamente el tipo
        zombies[i].tipoZombie=rand()%2;
        printf("Zombie %d - Tipo: %d\n", i, zombies[i].tipoZombie);

        //colocamos el zombie en la matriz
        colocarZombie(jardin, zombies[i].fila, zombies[i].columna);
        printf("Zombie colocado correctamente\n");
    }

    int turno=1;
    int contadorGirasoles=0;

    // definimos si el juego termino
    int juegoTerminado=0;

    // guardamos el tiempo inicial
    float tiempoAnterior=GetTime();
    printf("Comenzando el juego\n");

    // mantenemos la ventana abierta
    while(!WindowShouldClose()){

        // verificamos si paso un segundo
        if(GetTime()-tiempoAnterior>=1.0f && juegoTerminado==0){

            printf("\nTurno %d:\n", turno);

            //actualizamos el turno del juego
            actualizarTurno(jardin, zombies, cantidadZombies, &soles, &contadorGirasoles, &juegoTerminado, cortacesped);

            // imprimimos la matriz actualizada
            imprimirMatriz(jardin, filas, columnas);

            // actualizamos el tiempo y el turno
            tiempoAnterior=GetTime();
            turno++;
        }
        //dibujamos el jardin en la ventana
        dibujarJardin(jardin, filas, columnas, zombies, cantidadZombies);
    }

    // cerramos la ventana
    CloseWindow();

    // liberamos la memoria de la matriz
    liberarJardin(jardin, filas);

    return 0;
}
