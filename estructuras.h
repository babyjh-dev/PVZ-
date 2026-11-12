
#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

/* Tipos de plantas disponibles */
typedef enum {
    PLANTA_GIRASOL,
    PLANTA_LANZAGUISANTES,
    PLANTA_NUEZ
} TipoPlanta;


/* Tipos de zombies disponibles */
typedef enum {
    ZOMBIE_NORMAL,
    ZOMBIE_BANDERA
} TipoZombie;


/*
 * Códigos que representan los elementos dentro de la matriz.
 */
typedef enum {
    CELDA_VACIA = 0,
    CELDA_LANZAGUISANTES = 1,
    CELDA_GIRASOL = 2,
    CELDA_NUEZ = 3,
    CELDA_ZOMBIE = 4

    /* Se dejan pendientes hasta coordinar su implementación */
    /* CELDA_ZOMBIE_BANDERA = 5, */
    /* CELDA_OBSTACULO = 6 */
} TipoCelda;


/* Características y estadísticas de cada planta */
typedef struct {
    TipoPlanta tipo;

    int costo;
    int vida;

    int solesGenerados;
    float frecuenciaGeneracion;

    int dano;
    float frecuenciaAtaque;
} Planta;


/* Características y estadísticas de cada zombie */
typedef struct {
    TipoZombie tipo;

    int vida;
    int dano;

    float velocidad;
    float frecuenciaAtaque;
} Zombie;


/* Información del tablero y su matriz */
typedef struct {
    int filas;
    int columnas;

    int vidaCasa;

    int **matriz;
} Tablero;


/* Parámetros generales y configuración inicial del juego */
typedef struct {
    Tablero tablero;

    /* Recursos y tiempos generales */
    int solesIniciales;
    float frecuenciaBaseSoles;
    float tiempoPreparacion;
    float tiempoDescanso;

    /* Configuración de las oleadas */
    int cantidadOleadas;
    int zombiesPrimeraOleada;
    int incrementoZombies;

    /* Información de las plantas */
    Planta *plantas;
    int cantidadPlantas;

    /* Información de los zombies */
    Zombie *zombies;
    int cantidadZombies;

} Configuracion;

#endif

