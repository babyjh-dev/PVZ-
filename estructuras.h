
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
    /*lo dejo como comentario porque aun no esta acorde al resto del codigo*/
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


/* Todos los parametros que sea necesario ir agregando deacuerdo al avance de nuestro codigo */
typedef struct {
    Tablero tablero;

    int cantidadOleadas;
    int solesIniciales;
    float frecuenciaBaseSoles;
    float tiempoPreparacion;

    Planta *plantas;
    int cantidadPlantas;

    Zombie *zombies;
    int cantidadZombies;

} Configuracion;

#endif
