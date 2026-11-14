#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "configuracion.h"


int cargarConfiguracion(Configuracion *config, const char *nombreArchivo) {

    FILE *archivo;
    char seccion[30];
    char tipo[30];
    int caracter;

    if (config == NULL || nombreArchivo == NULL) {
        return 0;
    }

    /* Inicializamos la estructura */
    *config = (Configuracion){0};

    /* Abrimos el archivo en modo lectura */
    archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo de configuracion.\n");
        return 0;
    }

    /* Leemos la vida inicial de la casa */
    if (fscanf(archivo, "%29s", seccion) != 1 ||
        strcmp(seccion, "CASA") != 0) {
        goto error;
    }

    if (fscanf(archivo, "%d", &config->tablero.vidaCasa) != 1 ||
        config->tablero.vidaCasa <= 0) {
        goto error;
    }


    /* Leemos los parametros generales del juego */
    if (fscanf(archivo, "%29s", seccion) != 1 ||
        strcmp(seccion, "JUEGO") != 0) {
        goto error;
    }

    if (fscanf(archivo, "%d %f %f %f",
        &config->solesIniciales,
        &config->frecuenciaBaseSoles,
        &config->tiempoPreparacion,
        &config->tiempoDescanso) != 4) {
        goto error;
    }

    if (config->solesIniciales < 0 ||
        config->frecuenciaBaseSoles <= 0 ||
        config->tiempoPreparacion < 0 ||
        config->tiempoDescanso < 0) {
        goto error;
    }


    /* Leemos los parametros de las oleadas */
    if (fscanf(archivo, "%29s", seccion) != 1 ||
        strcmp(seccion, "OLEADAS") != 0) {
        goto error;
    }

    if (fscanf(archivo, "%d %d %d",
        &config->cantidadOleadas,
        &config->zombiesPrimeraOleada,
        &config->incrementoZombies) != 3) {
        goto error;
    }

    if (config->cantidadOleadas <= 0 ||
        config->zombiesPrimeraOleada <= 0 ||
        config->incrementoZombies < 0) {
        goto error;
    }


    /* Leemos la cantidad de plantas */
    if (fscanf(archivo, "%29s", seccion) != 1 ||
        strcmp(seccion, "PLANTAS") != 0) {
        goto error;
    }

    if (fscanf(archivo, "%d", &config->cantidadPlantas) != 1 ||
        config->cantidadPlantas <= 0) {
        goto error;
    }

    /* Reservamos memoria para las plantas */
    if ((size_t)config->cantidadPlantas >
        SIZE_MAX / sizeof(Planta)) {
        goto error;
    }

    config->plantas = malloc(
        (size_t)config->cantidadPlantas * sizeof(Planta)
    );

    if (config->plantas == NULL) {
        goto error;
    }


    /* Leemos las caracteristicas de cada planta */
    for (int i = 0; i < config->cantidadPlantas; i++) {

        Planta *planta = &config->plantas[i];

        if (fscanf(archivo, "%29s %d %d %d %f %d %f",
            tipo,
            &planta->costo,
            &planta->vida,
            &planta->solesGenerados,
            &planta->frecuenciaGeneracion,
            &planta->dano,
            &planta->frecuenciaAtaque) != 7) {
            goto error;
        }

        /* Identificamos el tipo de planta */
        if (strcmp(tipo, "GIRASOL") == 0) {
            planta->tipo = PLANTA_GIRASOL;
        }
        else if (strcmp(tipo, "LANZAGUISANTES") == 0) {
            planta->tipo = PLANTA_LANZAGUISANTES;
        }
        else if (strcmp(tipo, "NUEZ") == 0) {
            planta->tipo = PLANTA_NUEZ;
        }
        else {
            goto error;
        }

        /* Validamos sus caracteristicas */
        if (planta->costo < 0 ||
            planta->vida <= 0 ||
            planta->solesGenerados < 0 ||
            planta->frecuenciaGeneracion < 0 ||
            planta->dano < 0 ||
            planta->frecuenciaAtaque < 0) {
            goto error;
        }
    }


    /* Leemos la cantidad de zombies */
    if (fscanf(archivo, "%29s", seccion) != 1 ||
        strcmp(seccion, "ZOMBIES") != 0) {
        goto error;
    }

    if (fscanf(archivo, "%d", &config->cantidadZombies) != 1 ||
        config->cantidadZombies <= 0) {
        goto error;
    }

    /* Reservamos memoria para los zombies */
    if ((size_t)config->cantidadZombies >
        SIZE_MAX / sizeof(ZombieConfig)) {
        goto error;
    }

    config->zombies = malloc(
        (size_t)config->cantidadZombies * sizeof(ZombieConfig)
    );

    if (config->zombies == NULL) {
        goto error;
    }


    /* Leemos las caracteristicas de cada zombie */
    for (int i = 0; i < config->cantidadZombies; i++) {

        ZombieConfig *zombie= &config->zombies[i];

        if (fscanf(archivo, "%29s %d %d %f %f",
            tipo,
            &zombie->vida,
            &zombie->dano,
            &zombie->velocidad,
            &zombie->frecuenciaAtaque) != 5) {
            goto error;
        }

        /* Identificamos el tipo de zombie */
        if (strcmp(tipo, "NORMAL") == 0) {
            zombie->tipo = ZOMBIE_NORMAL;
        }
        else if (strcmp(tipo, "BANDERA") == 0) {
            zombie->tipo = ZOMBIE_BANDERA;
        }
        else {
            goto error;
        }

        /* Validamos sus caracteristicas */
        if (zombie->vida <= 0 ||
            zombie->dano < 0 ||
            zombie->velocidad <= 0 ||
            zombie->frecuenciaAtaque < 0) {
            goto error;
        }
    }


    /* Comprobamos que no existan datos adicionales */
    while ((caracter = fgetc(archivo)) != EOF) {
        if (!isspace((unsigned char)caracter)) {
            goto error;
        }
    }

    fclose(archivo);
    return 1;


/* Si ocurre un error, liberamos lo reservado */
error:

    printf("Error al leer la configuracion.\n");

    fclose(archivo);
    liberarConfiguracion(config);

    /* Dejamos la estructura en estado inicial */
    *config = (Configuracion){0};

    return 0;
}


/* Libera la memoria dinamica de la configuracion */
void liberarConfiguracion(Configuracion *config) {

    if (config == NULL) {
        return;
    }

    free(config->plantas);
    free(config->zombies);

    config->plantas = NULL;
    config->zombies = NULL;

    config->cantidadPlantas = 0;
    config->cantidadZombies = 0;
}
