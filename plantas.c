#include <stdio.h>

#include "plantas.h"
#include "estructuras.h"

// Compramos y colocamos una planta
int comprarPlanta(
    int **jardin,
    int *filas,
    int *columnas,
    int *soles,
    int *girasolComprado,
    int *filaPlanta,
    int *columnaPlanta
) {

    int opcion;
    int costo = 0;
    int tipoPlanta = 0;

    // Mostramos las plantas disponibles en la tienda
    printf("\n--- TIENDA ---\n");
    printf("Tienes %d soles\n", *soles);
    printf("1. Girasol - 50 soles\n");
    printf("2. Lanzaguisantes - 100 soles\n");
    printf("3. Nuez - 150 soles\n");
    printf("4. Gato espinoso - 125 soles\n");
    printf("5. Lanzaguisantes de fuego - 175 soles\n");
    printf("6. Nuez reforzada - 200 soles\n");

    // Pedimos la planta que queremos comprar
    printf("Ingrese la planta que desea comprar: ");

    if (scanf("%d", &opcion) != 1) {
        printf("Opcion no valida\n");
        return 0;
    }

    // Identificamos la planta elegida y su costo
    if (opcion == 1) {
        costo = 50;
        tipoPlanta = CELDA_GIRASOL;
    }
    else if (opcion == 2) {
        costo = 100;
        tipoPlanta = CELDA_LANZAGUISANTES;
    }
    else if (opcion == 3) {
        costo = 150;
        tipoPlanta = CELDA_NUEZ;
    }
    else if (opcion == 4) {
        costo = 125;
        tipoPlanta = CELDA_GATOESPINOSO;
    }
    else if (opcion == 5) {
        costo = 175;
        tipoPlanta = CELDA_LANZAGUISANTESFUEGO;
    }
    else if (opcion == 6) {
        costo = 200;
        tipoPlanta = CELDA_NUEZREFORZADA;
    }

    // Verificamos si la opcion ingresada es valida
    if (costo == 0) {
        printf("Opcion no valida\n");
        return 0;
    }

    // Verificamos si tenemos suficientes soles para comprar
    if (*soles < costo) {
        printf("No tienes suficientes soles\n");
        return 0;
    }

    // Pedimos la fila donde queremos colocar la planta
    printf("Ingrese la fila donde quiere colocar la planta (0 a %d): ",
           *filas - 1);

    if (scanf("%d", filaPlanta) != 1) {
        printf("Fila no valida\n");
        return 0;
    }

    // Pedimos la columna donde queremos colocar la planta
    printf("Ingrese la columna donde quiere colocar la planta (0 a %d): ",
           *columnas - 1);

    if (scanf("%d", columnaPlanta) != 1) {
        printf("Columna no valida\n");
        return 0;
    }

    // Verificamos que la posicion ingresada este dentro del jardin
    if (*filaPlanta < 0 || *filaPlanta >= *filas ||
        *columnaPlanta < 0 || *columnaPlanta >= *columnas) {

        printf("La posicion esta fuera del jardin\n");
        return 0;
    }

    // Comprobamos que la casilla elegida este vacia
    if (jardin[*filaPlanta][*columnaPlanta] != 0) {
        printf("La posicion esta ocupada\n");
        return 0;
    }

    // Descontamos el costo de la planta de los soles disponibles
    *soles -= costo;

    // Colocamos la planta en la posicion elegida
    jardin[*filaPlanta][*columnaPlanta] = tipoPlanta;

    printf("Compra realizada!\n");
    printf("Soles restantes: %d\n", *soles);

    // Registramos si el jugador compro un girasol
    if (tipoPlanta == 2) {
        *girasolComprado = 1;
    }

    return tipoPlanta;
}
