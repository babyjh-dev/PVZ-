#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "partida.h"
#include "jardin.h"


// Libera la memoria utilizada por una partida temporal
static void liberarPartidaTemporal(Partida *partida){

    // Comprobamos que la partida exista
    if(partida == NULL){
        return;
    }

    // Liberamos la memoria del jardin
    liberarJardin(partida->jardin, &partida->filas);

    // Liberamos la memoria de los zombies y cortacespedes
    free(partida->zombies);
    free(partida->cortacesped);

    // Reiniciamos la estructura para dejarla vacia
    *partida = (Partida){0};
}


// Guarda el estado actual de la partida en un archivo
int guardarPartida(const char *nombreArchivo, Partida *partida){

    // Comprobamos que los parametros sean validos
    if(nombreArchivo == NULL || partida == NULL){
        return 0;
    }

    // Comprobamos que la partida tenga todos los datos necesarios
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

    // Abrimos el archivo en modo escritura
    FILE *archivo = fopen(nombreArchivo, "w");

    if(archivo == NULL){
        printf("Error al crear el archivo de partida.\n");
        return 0;
    }

    // Guardamos el identificador y la version del archivo
    fprintf(archivo, "PVZ_SAVE 1\n");

    // Guardamos las dimensiones del jardin
    fprintf(
        archivo,
        "%d %d\n",
        partida->filas,
        partida->columnas
    );

    // Guardamos los datos generales de la partida
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

    // Guardamos la cantidad y capacidad de zombies
    fprintf(
        archivo,
        "%d %d\n",
        partida->cantidadZombies,
        partida->capacidadZombies
    );

    // Guardamos todos los valores de la matriz del jardin
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

    // Guardamos el estado de los cortacespedes
    for(int i = 0; i < partida->filas; i++){

        fprintf(
            archivo,
            "%d ",
            partida->cortacesped[i]
        );
    }

    fprintf(archivo, "\n");

    // Guardamos los datos de cada zombie
    for(int i = 0; i < partida->cantidadZombies; i++){

        // Obtenemos los datos del zombie actual
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

    // Comprobamos si ocurrio algun error durante la escritura
    int correcto = !ferror(archivo);

    // Cerramos el archivo y comprobamos si se cerro correctamente
    if(fclose(archivo) != 0){
        correcto = 0;
    }

    // Informamos si ocurrio un error al guardar
    if(!correcto){
        printf("Error al guardar la partida.\n");
        return 0;
    }

    printf("Partida guardada correctamente.\n");
    return 1;
}


// Carga una partida guardada desde un archivo
int cargarPartida(const char *nombreArchivo, Partida *partida){

    // Comprobamos que los parametros sean validos
    if(nombreArchivo == NULL || partida == NULL){
        return 0;
    }

    // Creamos una estructura temporal para cargar los datos
    Partida nueva = {0};

    // Abrimos el archivo en modo lectura
    FILE *archivo = fopen(nombreArchivo, "r");

    if(archivo == NULL){
        printf("Error: no se encontro el archivo de partida.\n");
        return 0;
    }

    char identificador[16];
    int version;

    // Leemos y comprobamos el identificador y la version del archivo
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

    // Leemos las dimensiones del jardin
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

    // Comprobamos que las dimensiones permitan reservar memoria
    if((size_t)nueva.filas > SIZE_MAX / sizeof(int *) ||
       (size_t)nueva.columnas > SIZE_MAX / sizeof(int)){

        printf("Error: dimensiones demasiado grandes.\n");
        fclose(archivo);
        return 0;
    }

    // Leemos los datos generales de la partida
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

    // Leemos la cantidad y capacidad de zombies
    if(fscanf(
        archivo,
        "%d %d",
        &nueva.cantidadZombies,
        &nueva.capacidadZombies
    ) != 2 || nueva.cantidadZombies < 0 || nueva.capacidadZombies <= 0 || nueva.cantidadZombies > nueva.capacidadZombies){

        printf("Error en los datos de los zombies.\n");
        fclose(archivo);
        return 0;
    }

    // Comprobamos que la capacidad de zombies permita reservar memoria
    if((size_t)nueva.capacidadZombies > SIZE_MAX / sizeof(Zombie)){

        printf("Error: capacidad de zombies demasiado grande.\n");
        fclose(archivo);
        return 0;
    }

    // Reservamos memoria para crear el jardin
    nueva.jardin = crearJardin(&nueva.filas, &nueva.columnas);

    if(nueva.jardin == NULL){
        printf("Error al reservar memoria para el jardin.\n");
        fclose(archivo);
        return 0;
    }

    // Recuperamos los valores de la matriz del archivo
    for(int i = 0; i < nueva.filas; i++){
        for(int j = 0; j < nueva.columnas; j++){

            // Leemos cada celda y comprobamos que su valor sea valido
            if(fscanf(
                archivo,
                "%d",
                &nueva.jardin[i][j]
            ) != 1 || nueva.jardin[i][j] < CELDA_VACIA || nueva.jardin[i][j] > CELDA_ZOMBIE){

                printf("Error al recuperar la matriz.\n");

                fclose(archivo);
                liberarPartidaTemporal(&nueva);
                return 0;
            }
        }
    }

    // Comprobamos que se pueda reservar memoria para los cortacespedes
    if((size_t)nueva.filas > SIZE_MAX / sizeof(int)){
        fclose(archivo);
        liberarPartidaTemporal(&nueva);
        return 0;
    }

    // Reservamos memoria para los cortacespedes
    nueva.cortacesped = malloc(
        nueva.filas * sizeof(int)
    );

    if(nueva.cortacesped == NULL){

        printf("Error al reservar los cortacespedes.\n");

        fclose(archivo);
        liberarPartidaTemporal(&nueva);
        return 0;
    }

    // Recuperamos el estado de cada cortacesped
    for(int i = 0; i < nueva.filas; i++){

        // Leemos el valor y comprobamos que sea 0 o 1
        if(fscanf(
            archivo,
            "%d",
            &nueva.cortacesped[i]
        ) != 1 || (nueva.cortacesped[i] != 0 && nueva.cortacesped[i] != 1)){

            printf("Error al recuperar los cortacespedes.\n");

            fclose(archivo);
            liberarPartidaTemporal(&nueva);
            return 0;
        }
    }

    // Reservamos memoria para almacenar los zombies
    nueva.zombies = malloc(nueva.capacidadZombies * sizeof(Zombie));

    if(nueva.zombies == NULL){

        printf("Error al reservar memoria para los zombies.\n");

        fclose(archivo);
        liberarPartidaTemporal(&nueva);
        return 0;
    }

    // Recuperamos los datos de cada zombie
    for(int i = 0; i < nueva.cantidadZombies; i++){

        // Accedemos al zombie actual
        Zombie *z = &nueva.zombies[i];

        // Leemos todos los campos del zombie
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

        // Comprobamos que la posicion y el estado del zombie sean validos
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

    // Comprobamos que el archivo no tenga datos adicionales
    char datoExtra[2];

    if(fscanf(archivo, "%1s", datoExtra) == 1){

        printf("Error: el archivo contiene datos adicionales.\n");

        fclose(archivo);
        liberarPartidaTemporal(&nueva);
        return 0;
    }

    // Cerramos el archivo despues de terminar la lectura
    fclose(archivo);

    // Si la carga fue correcta, transferimos los datos a la partida original
    *partida = nueva;

    printf("Partida cargada correctamente.\n");
    return 1;
}
