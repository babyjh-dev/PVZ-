#ifndef estructuras_h
#define estructuras_h

// Tipos de plantas disponibles
typedef enum {
    PLANTA_GIRASOL,
    PLANTA_LANZAGUISANTES,
    PLANTA_NUEZ,
    PLANTA_GATOESPINOSO,
    PLANTA_LANZAGUISANTESFUEGO,
    PLANTA_NUEZREFORZADA
} TipoPlanta;

//Tipos de zombies disponibles
typedef enum {
    ZOMBIE_NORMAL,
    ZOMBIE_BANDERA,
    ZOMBIE_CONO,
    ZOMBIE_CUBETA
} TipoZombie;

// Códigos que representan los elementos dentro de la matriz
typedef enum {
    CELDA_VACIA = 0,
    CELDA_LANZAGUISANTES = 1,
    CELDA_GIRASOL = 2,
    CELDA_NUEZ = 3,
    CELDA_ZOMBIE = 4,
    CELDA_GATOESPINOSO = 5,
    CELDA_LANZAGUISANTESFUEGO = 6,
    CELDA_NUEZREFORZADA = 7
} TipoCelda;

// Características y estadísticas de cada planta
typedef struct {
    TipoPlanta tipo;
    int costo;
    int vida;
    int solesGenerados;
    float frecuenciaGeneracion;
    int dano;
    float frecuenciaAtaque;
} Planta;

// Características y estadísticas de cada zombie
typedef struct {
    TipoZombie tipo;
    int vida;
    int dano;
    float velocidad;
    float frecuenciaAtaque;
} ZombieConfig;

// Estado individual de cada zombie durante la partida
typedef struct {
    int fila;
    int columna;
    int vida;
    int activo;
    int contadorAtaque;
    int contadorComida;
    int tipoZombie;
    int contadorMovimiento;
    //caracteristicas el zombie
    int dano;
    float velocidad;
    float frecuenciaAtaque;
} Zombie;

// Información del tablero y su matriz
typedef struct {
    int filas;
    int columnas;
    int vidaCasa;
    int **matriz;
} Tablero;

// Parámetros generales y configuración inicial del juego
typedef struct {
    Tablero tablero;
    int solesIniciales;
    float frecuenciaBaseSoles;
    float tiempoPreparacion;
    float tiempoDescanso;
    int cantidadOleadas;
    int zombiesPrimeraOleada;
    int incrementoZombies;
    Planta *plantas;
    int cantidadPlantas;
    ZombieConfig *zombies;
    int cantidadZombies;
} Configuracion;

// Estado de una partida en curso
typedef struct {
    int filas;
    int columnas;
    int **jardin;
    Zombie *zombies;
    int cantidadZombies;
    int capacidadZombies;
    int soles;
    int *cortacesped;
    int turno;
    int contadorGirasoles;
    int oleadaActual;
    int zombiesPorOleada;//cantidad de zombies que debe de haber por oleada
    int zombiesGenerados;//cantidad de zombies que ya han aparecido
    int juegoTerminado;
} Partida;

#endif
