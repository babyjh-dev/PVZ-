#ifndef zombies_h
#define zombies_h

#define MAX_ZOMBIES 3

//guardamos los datos de cada zombie y asi cada uno pora tener su propia fila, columna y vida
typedef struct{
    int fila;
    int columna;
    int vida;
    int activo;
    int contadorAtaque;
    int contadorComida;
} Zombie;

//funciones de los zombies
void colocarZombie(int**jardin, int fila, int columna);
void moverZombie(int**jardin, Zombie *zombie);

#endif
