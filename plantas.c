#include <stdio.h>
#include "plantas.h"
#include "estructuras.h"

// Compramos y colocamos una planta.
int comprarPlanta(
    int **jardin,
    int filas,
    int columnas,
    int *soles,
    int *girasolComprado,
    int *filaPlanta,
    int *columnaPlanta
) {

    int opcion;
    int costo = 0;
    int tipoPlanta = 0;

    // Mostramos las plantas disponibles.
    printf("\n--- TIENDA ---\n");
    printf("Tienes %d soles\n", *soles);
    printf("1. Girasol - 50 soles\n");
    printf("2. Lanzaguisantes - 100 soles\n");
    printf("3. Nuez - 150 soles\n");
    printf("4. Gato espinoso - 125 soles\n");
    printf("5. Lanzaguisantes de fuego - 175 soles\n");
    printf("6. Nuez reforzada - 200 soles\n");

    // Pedimos la planta que queremos comprar.
    printf("Ingrese la planta que desea comprar: ");

    if (scanf("%d", &opcion) != 1) {
        printf("Opcion no valida\n");
        return 0;
    }

    // Identificamos la planta y su costo.
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
        costo = 150;
        tipoPlanta = CELDA_GATOESPINOSO;
    }
    else if (opcion == 5) {
        costo = 150;
        tipoPlanta = CELDA_LANZAGUISANTESFUEGO;
    }
    else if (opcion == 6) {
        costo = 150;
        tipoPlanta = CELDA_NUEZREFORZADA;
    }

    // Verificamos si la opcion es valida.
    if (costo == 0) {
        printf("Opcion no valida\n");
        return 0;
    }

    // Verificamos si tenemos suficientes soles.
    if (*soles < costo) {
        printf("No tienes suficientes soles\n");
        return 0;
    }

    // Pedimos la posicion donde queremos colocar la planta.
    printf("Ingrese la fila donde quiere colocar la planta (0 a %d): ",
           filas - 1);

    if (scanf("%d", filaPlanta) != 1) {
        printf("Fila no valida\n");
        return 0;
    }

    printf("Ingrese la columna donde quiere colocar la planta (0 a %d): ",
           columnas - 1);

    if (scanf("%d", columnaPlanta) != 1) {
        printf("Columna no valida\n");
        return 0;
    }

    // Verificamos que la posicion exista y este vacia.
    if (*filaPlanta < 0 || *filaPlanta >= filas ||
        *columnaPlanta < 0 || *columnaPlanta >= columnas) {

        printf("La posicion esta fuera del jardin\n");
        return 0;
    }

    if (jardin[*filaPlanta][*columnaPlanta] != 0) {
        printf("La posicion esta ocupada\n");
        return 0;
    }

    // Descontamos el costo de la planta.
    *soles -= costo;

    // Colocamos la planta en el jardin.
    jardin[*filaPlanta][*columnaPlanta] = tipoPlanta;

    printf("Compra realizada!\n");
    printf("Soles restantes: %d\n", *soles);

    // Registramos si compramos un girasol.
    if (tipoPlanta == 2) {
        *girasolComprado = 1;
    }

    return tipoPlanta;
}
