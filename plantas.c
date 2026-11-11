#include <stdio.h>
#include "plantas.h"

// compramos y colocamos una planta
int comprarPlanta(int **jardin, int filas, int columnas, int *soles, int *girasolComprado, int *filaPlanta, int *columnaPlanta){

    int opcion;
    int costo=0;
    int tipoPlanta=0;

    // mostramos las plantas que podemos comprar
    printf("\n--- TIENDA ---\n");
    printf("Tienes %d soles\n", *soles);
    printf("1. Girasol - 50 soles\n");
    printf("2. Lanzaguisantes - 100 soles\n");
    printf("3. Nuez - 150 soles\n");

    // pedimos la planta que queremos comprar
    printf("Ingrese la planta que desea comprar: ");
    scanf("%d", &opcion);

    // verificamos que planta queremos comprar
    if(opcion==1){
        costo=50;
        tipoPlanta=2;
    }
    else if(opcion==2){
        costo=100;
        tipoPlanta=1;
    }
    else if(opcion==3){
        costo=150;
        tipoPlanta=3;
    }

    // verificamos si tenemos suficientes soles
    if(costo>0 && *soles>=costo){

        // descontamos el costo de la planta
        *soles=*soles-costo;

        printf("Compra realizada!\n");
        printf("Soles restantes: %d\n", *soles);

        // pedimos la posicion donde queremos colocar la planta
        printf("Ingrese la fila donde quiere colocar la planta (0 a 4): ");
        scanf("%d", filaPlanta);

        printf("Ingrese la columna donde quiere colocar la planta (0 a 8): ");
        scanf("%d", columnaPlanta);

        // verificamos que la posicion este dentro del jardin y este vacia
        if(*filaPlanta>=0 && *filaPlanta<filas &&
           *columnaPlanta>=0 && *columnaPlanta<columnas &&
           jardin[*filaPlanta][*columnaPlanta]==0){

            // colocamos la planta en la matriz
            jardin[*filaPlanta][*columnaPlanta]=tipoPlanta;

            // verificamos si compramos un girasol
            if(tipoPlanta==2){
                *girasolComprado=1;
            }

        }
        else{
            printf("La posicion no es valida o esta ocupada\n");

            // devolvemos los soles si no se pudo colocar la planta
            *soles=*soles+costo;

            tipoPlanta=0;
        }
    }
    else{
        printf("No tienes suficientes soles o la opcion no es valida\n");
    }

    return tipoPlanta;
}
