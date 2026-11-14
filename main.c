#include <stdio.h>
#include <stdlib.h> //para usar malloc y free
#include <raylib.h> //para la visual
#include <time.h> //para usar time
#include "configuracion.h"
#include "jardin.h"
#include "plantas.h"
#include "zombies.h"
#include "juego.h"


int main(){
    srand(time(NULL));//iniciamos los numeros aleatorios
    //cargamos la configuracion del juego
    Configuracion config={0};

    if(cargarConfiguracion(&config, "configuracion.txt")==0){
        printf("error al cargar la configuracion\n");
        return 1;
    }
    
    int filas = 0;
    int columnas = 0;
    int **jardin = NULL;
    
    if (!leerJardin("jardin.txt", &jardin, &filas, &columnas)) {
        printf("Error al cargar el jardin\n");
        liberarConfiguracion(&config);
        return 1;
    }
    // abrimos la ventana del juego
    InitWindow(900, 600, "Plants vs Zombies");
    SetTargetFPS(60);

    int soles = config.solesIniciales;
    
    printf("El juego comenzo\n");
    int girasolComprado=0;

    int filaPlanta=-1;
    int columnaPlanta=-1;

    //preguntamos si queremos seguir comprando plantas
    int seguirComprando=1;

    while(seguirComprando==1){
    printf("Entrando a la tienda\n");
        //compramos y colocamos una planta 
    comprarPlanta(jardin, filas, columnas, &soles,&girasolComprado, &filaPlanta, &columnaPlanta);
        //preguntamos si queremos comprar otra planta
        printf("\nQuieres comprar otra planta? (1:SI/0:NO): ");
        scanf("%d", &seguirComprando);
    }
    //guardamos los datos de los zombies
    Zombie zombies[MAX_ZOMBIES];
    printf("sali de la tienda correctamente\n");

    int oleadaActual = 1;
    int zombiesPorOleada = config.zombiesPrimeraOleada;
    
    // Calculamos la capacidad necesaria para la oleada más grande
    int capacidadZombies = config.zombiesPrimeraOleada +
        (config.cantidadOleadas - 1) * config.incrementoZombies;
    
    // Reservamos memoria dinámicamente
    Zombie *zombies = malloc(capacidadZombies * sizeof(Zombie));
    
    if (zombies == NULL) {
        printf("Error al reservar memoria para los zombies\n");
    
        liberarJardin(jardin, filas);
        liberarConfiguracion(&config);
        CloseWindow();
    
        return 1;
    }
    
    int cantidadZombies = 0;
    
    generarOleadas(jardin, zombies, &cantidadZombies,
                   zombiesPorOleada, filas);
    //creamos un cortacesped para cada fila y cuando se utilice cambia a 0
    int cortacesped[filas];
    
    for (int i = 0; i < filas; i++) {
        cortacesped[i] = 1;
    }
    int turno=1;
    int contadorGirasoles=0;

    // definimos si el juego termino
    int juegoTerminado=0;

    // guardamos el tiempo inicial
    float tiempoAnterior=GetTime();
    printf("Comenzando el juego\n");

    // mantenemos la ventana abierta
    while(!WindowShouldClose()){

        // verificamos si paso un segundo
        if(GetTime()-tiempoAnterior>=1.0f && juegoTerminado==0){

            printf("\nTurno %d:\n", turno);

            //actualizamos el turno del juego
            actualizarTurno(jardin, zombies, cantidadZombies, &soles, &contadorGirasoles, &juegoTerminado, cortacesped);
            // contamos los zombies que siguen activos
    int zombiesActivos=0;

    for(int i=0; i<cantidadZombies; i++){
        if(zombies[i].activo==1){
            zombiesActivos++;
            printf("Zombies activos: %d\n", zombiesActivos);
        }
    }

    // verificamos si termino la oleada
    if(zombiesActivos==0 && juegoTerminado==0){

       if (oleadaActual < config.cantidadOleadas) {
        
            oleadaActual++;
        
            zombiesPorOleada += config.incrementoZombies;
        
            cantidadZombies = 0;
        
            generarOleadas(jardin, zombies, &cantidadZombies,
                           zombiesPorOleada, filas);
        
            printf("Comienza la oleada %d con %d zombies\n",
                   oleadaActual, zombiesPorOleada);

    } else {
    
        printf("FELICIDADES! GANASTE EL JUEGO\n");
        juegoTerminado = 1;
    }
}
            // imprimimos la matriz actualizada
            imprimirMatriz(jardin, filas, columnas);

            // actualizamos el tiempo y el turno
            tiempoAnterior=GetTime();
            turno++;
        }
        //dibujamos el jardin en la ventana
        dibujarJardin(jardin, filas, columnas, zombies, cantidadZombies);
    }

    // cerramos la ventana
    CloseWindow();

    liberarJardin(jardin, filas);
    free(zombies);
    liberarConfiguracion(&config);
    
    jardin = NULL;
    zombies = NULL;
    
    return 0;


}
