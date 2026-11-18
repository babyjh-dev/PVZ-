#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "partida.h"
#include "jardin.h"


/* Libera la memoria de una partida auxiliar. */
static void liberarPartidaTemporal(Partida *partida){

    if(partida == NULL){
        return;
    }

    liberarJardin(partida->jardin, &partida->filas);

    free(partida->zombies);
    free(partida->cortacesped);

    *partida = (Partida){0};
}


/* Guarda el estado actual de la partida. */
int guardarPartida(const char *nombreArchivo, Partida *partida){

    if(nombreArchivo == NULL || partida == NULL){
        return 0;
    }

    if(partida->jardin == NULL ||
       partida->zombies == NULL ||
       partida->cortacesped == NULL ||
       partida->filas <= 0 ||
       partida->columnas <= 0 ||
       partida->cantidadZombies < 0 ||
       partida->capacidadZombies < partida->cantidadZombies ||
       partida->capacidadZombies <= 0){
        return 0;
    }

    FILE *archivo = fopen(nombreArchivo, "w");

    if(archivo == NULL){
        printf("Error al crear el archivo de partida.\n");
        return 0;
    }

    // Identificador y version del archivo.
    fprintf(archivo, "PVZ_SAVE 1\n");

    // Dimensiones del jardin.
    fprintf(
        archivo,
        "%d %d\n",
        partida->filas,
        partida->columnas
    );

    // Datos generales de la partida.
    fprintf(
        archivo,
        "%d %d %d %d %d %d\n",
        partida->soles,
        partida->turno,
        partida->contadorGirasoles,
        partida->oleadaActual,
        partida->zombiesPorOleada,
        partida->juegoTerminado
    );

    // Cantidad y capacidad de zombies.
    fprintf(
        archivo,
        "%d %d\n",
        partida->cantidadZombies,
        partida->capacidadZombies
    );

    // Guardamos la matriz del jardin.
    for(int i = 0; i < partida->filas; i++){

        for(int j = 0; j < partida->columnas; j++){

            fprintf(
                archivo,
                "%d ",
                partida->jardin[i][j]
            );
        }

        fprintf(archivo, "\n");
    }

    // Guardamos el estado de los cortacespedes.
    for(int i = 0; i < partida->filas; i++){

        fprintf(
            archivo,
            "%d ",
            partida->cortacesped[i]
        );
    }

    fprintf(archivo, "\n");

    // Guardamos todos los datos de cada zombie.
    for(int i = 0; i < partida->cantidadZombies; i++){

        Zombie z = partida->zombies[i];

        fprintf(
            archivo,
            "%d %d %d %d %d %d %d %d\n",
            z.fila,
            z.columna,
            z.vida,
            z.activo,
            z.contadorAtaque,
            z.contadorComida,
            z.tipoZombie,
            z.contadorMovimiento
        );
    }

    // Comprobamos que la escritura haya sido correcta.
    int correcto = !ferror(archivo);

    if(fclose(archivo) != 0){
        correcto = 0;
    }

    if(!correcto){
        printf("Error al guardar la partida.\n");
        return 0;
    }

    printf("Partida guardada correctamente.\n");
    return 1;
}


/* Carga una partida desde un archivo */
int cargarPartida(const char *nombreArchivo, Partida *partida){

    if(nombreArchivo == NULL || partida == NULL){
        return 0;
    }

    // Usamos una estructura temporal para evitar dejar
    // la partida parcialmente cargada si ocurre un error.
    Partida nueva = {0};

    FILE *archivo = fopen(nombreArchivo, "r");

    if(archivo == NULL){
        printf("Error: no se encontro el archivo de partida.\n");
        return 0;
    }

    char identificador[16];
    int version;

    // Comprobamos el identificador y la version.
    if(fscanf(archivo, "%15s %d", identificador, &version) != 2 ||
       identificador[0] == '\0' ||
       identificador[15] != '\0' ||
       identificador[0] == ' ' ||
       version != 1 ||
       identificador[0] != 'P' ||
       identificador[1] != 'V' ||
       identificador[2] != 'Z' ||
       identificador[3] != '_' ||
       identificador[4] != 'S' ||
       identificador[5] != 'A' ||
       identificador[6] != 'V' ||
       identificador[7] != 'E' ||
       identificador[8] != '\0'){

        printf("Error: archivo de partida invalido.\n");
        fclose(archivo);
        return 0;
    }

    // Leemos las dimensiones.
    if(fscanf(
        archivo,
        "%d %d",
        &nueva.filas,
        &nueva.columnas
    ) != 2 ||
       nueva.filas <= 0 ||
       nueva.columnas <= 0){

        printf("Error: dimensiones del jardin invalidas.\n");
        fclose(archivo);
        return 0;
    }

    // Comprobamos que las dimensiones no provoquen
    // desbordamientos al reservar memoria.
    if((size_t)nueva.filas > SIZE_MAX / sizeof(int *) ||
       (size_t)nueva.columnas > SIZE_MAX / sizeof(int)){

        printf("Error: dimensiones demasiado grandes.\n");
        fclose(archivo);
        return 0;
    }

    // Leemos los datos generales.
    if(fscanf(
        archivo,
        "%d %d %d %d %d %d",
        &nueva.soles,
        &nueva.turno,
        &nueva.contadorGirasoles,
        &nueva.oleadaActual,
        &nueva.zombiesPorOleada,
        &nueva.juegoTerminado
    ) != 6){

        printf("Error al leer los datos generales.\n");
        fclose(archivo);
        return 0;
    }

    // Leemos la cantidad y capacidad de zombies.
    if(fscanf(
        archivo,
        "%d %d",
        &nueva.cantidadZombies,
        &nueva.capacidadZombies
    ) != 2 ||
       nueva.cantidadZombies < 0 ||
       nueva.capacidadZombies <= 0 ||
       nueva.cantidadZombies > nueva.capacidadZombies){

        printf("Error en los datos de los zombies.\n");
        fclose(archivo);
        return 0;
    }

    if((size_t)nueva.capacidadZombies >
       SIZE_MAX / sizeof(Zombie)){

        printf("Error: capacidad de zombies demasiado grande.\n");
        fclose(archivo);
        return 0;
    }

    // Reservamos memoria para el jardin.
    nueva.jardin = crearJardin(
        &nueva.filas,
        &nueva.columnas
    );

    if(nueva.jardin == NULL){

        printf("Error al reservar memoria para el jardin.\n");
        fclose(archivo);
        return 0;
    }

    // Recuperamos los valores de la matriz.
    for(int i = 0; i < nueva.filas; i++){

        for(int j = 0; j < nueva.columnas; j++){

            if(fscanf(
                archivo,
                "%d",
                &nueva.jardin[i][j]
            ) != 1 ||
               nueva.jardin[i][j] < CELDA_VACIA ||
               nueva.jardin[i][j] > CELDA_ZOMBIE){

                printf("Error al recuperar la matriz.\n");

                fclose(archivo);
                liberarPartidaTemporal(&nueva);
                return 0;
            }
        }
    }

    // Reservamos memoria para los cortacespedes.
    if((size_t)nueva.filas > SIZE_MAX / sizeof(int)){
        fclose(archivo);
        liberarPartidaTemporal(&nueva);
        return 0;
    }

    nueva.cortacesped = malloc(
        nueva.filas * sizeof(int)
    );

    if(nueva.cortacesped == NULL){

        printf("Error al reservar los cortacespedes.\n");

        fclose(archivo);
        liberarPartidaTemporal(&nueva);
        return 0;
    }

    // Recuperamos el estado de los cortacespedes.
    for(int i = 0; i < nueva.filas; i++){

        if(fscanf(
            archivo,
            "%d",
            &nueva.cortacesped[i]
        ) != 1 ||
           (nueva.cortacesped[i] != 0 &&
            nueva.cortacesped[i] != 1)){

            printf("Error al recuperar los cortacespedes.\n");

            fclose(archivo);
            liberarPartidaTemporal(&nueva);
            return 0;
        }
    }

    // Reservamos memoria para los zombies.
    nueva.zombies = malloc(
        nueva.capacidadZombies * sizeof(Zombie)
    );

    if(nueva.zombies == NULL){

        printf("Error al reservar memoria para los zombies.\n");

        fclose(archivo);
        liberarPartidaTemporal(&nueva);
        return 0;
    }

    // Recuperamos todos los campos de cada zombie.
    for(int i = 0; i < nueva.cantidadZombies; i++){

        Zombie *z = &nueva.zombies[i];

        if(fscanf(
            archivo,
            "%d %d %d %d %d %d %d %d",
            &z->fila,
            &z->columna,
            &z->vida,
            &z->activo,
            &z->contadorAtaque,
            &z->contadorComida,
            &z->tipoZombie,
            &z->contadorMovimiento
        ) != 8){

            printf("Error al recuperar los zombies.\n");

            fclose(archivo);
            liberarPartidaTemporal(&nueva);
            return 0;
        }

        // Validamos la posicion y el estado del zombie.
        if(z->fila < 0 ||
           z->fila >= nueva.filas ||
           z->columna < 0 ||
           z->columna >= nueva.columnas ||
           (z->activo != 0 && z->activo != 1) ||
           (z->tipoZombie != ZOMBIE_NORMAL &&
            z->tipoZombie != ZOMBIE_BANDERA)){

            printf("Error: datos de zombie invalidos.\n");

            fclose(archivo);
            liberarPartidaTemporal(&nueva);
            return 0;
        }
    }

    // Comprobamos que no existan datos adicionales.
    char datoExtra[2];

    if(fscanf(archivo, "%1s", datoExtra) == 1){

        printf("Error: el archivo contiene datos adicionales.\n");

        fclose(archivo);
        liberarPartidaTemporal(&nueva);
        return 0;
    }

    fclose(archivo);

    // La carga fue exitosa. Transferimos los datos.
    *partida = nueva;

    printf("Partida cargada correctamente.\n");
    return 1;
}
