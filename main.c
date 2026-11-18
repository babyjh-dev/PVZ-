
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <time.h>

#include "configuracion.h"
#include "jardin.h"
#include "plantas.h"
#include "zombies.h"
#include "juego.h"

int main(void) {

    srand(time(NULL));

    // Cargamos la configuracion del juego.
    Configuracion config = {0};

    if (!cargarConfiguracion(&config, "configuracion.txt")) {
        printf("Error al cargar la configuracion\n");
        return 1;
    }

    // Creamos e inicializamos la partida.
    Partida partida = {0};

    // Cargamos el jardin dinamicamente.
    partida.jardin = leerJardin(
        "jardin.txt",
        &partida.filas,
        &partida.columnas
    );

    if (partida.jardin == NULL) {
        printf("Error al cargar el jardin\n");
        liberarConfiguracion(&config);
        return 1;
    }

    int filas = partida.filas;
    int columnas = partida.columnas;

    // Creamos una matriz para los contadores de ataque.
    int **contadoresAtaque = crearJardin(&filas, &columnas);

    if (contadoresAtaque == NULL) {
        printf("Error al crear los contadores de ataque\n");
        liberarJardin(partida.jardin, &filas);
        liberarConfiguracion(&config);
        return 1;
    }

    // Calculamos la capacidad necesaria para la oleada mas grande.
    int capacidadZombies = config.zombiesPrimeraOleada +
        (config.cantidadOleadas - 1) * config.incrementoZombies;

    if (capacidadZombies <= 0) {
        printf("Error: capacidad de zombies no valida\n");
        liberarJardin(contadoresAtaque, &filas);
        liberarJardin(partida.jardin, &filas);
        liberarConfiguracion(&config);
        return 1;
    }

    // Guardamos la capacidad y reservamos memoria para los zombies.
    partida.capacidadZombies = capacidadZombies;
    partida.zombies = malloc(partida.capacidadZombies * sizeof(Zombie));

    if (partida.zombies == NULL) {
        printf("Error al reservar memoria para los zombies\n");
        liberarJardin(contadoresAtaque, &filas);
        liberarJardin(partida.jardin, &filas);
        liberarConfiguracion(&config);
        return 1;
    }

    // Reservamos memoria para los cortacespedes.
    partida.cortacesped = malloc(filas * sizeof(int));

    if (partida.cortacesped == NULL) {
        printf("Error al reservar memoria para los cortacespedes\n");
        free(partida.zombies);
        liberarJardin(contadoresAtaque, &filas);
        liberarJardin(partida.jardin, &filas);
        liberarConfiguracion(&config);
        return 1;
    }

    // Inicializamos los cortacespedes.
    for (int i = 0; i < filas; i++) {
        partida.cortacesped[i] = 1;
    }

    // Inicializamos los valores iniciales de la partida.
    partida.soles = config.solesIniciales;

    partida.oleadaActual = 1;
    partida.zombiesPorOleada = config.zombiesPrimeraOleada;
    partida.cantidadZombies = 0;
    partida.zombiesGenerados = 0;

    partida.turno = 1;
    partida.contadorGirasoles = 0;
    partida.juegoTerminado = 0;
    //variables para el entretiempo entre oleadas
    int entreOleadas = 0;
    float tiempoInicipEntreOleadas = 0;


    // Abrimos la ventana del juego.
    InitWindow(900, 600, "Plants vs Zombies");
    SetTargetFPS(60);

    printf("El juego comenzo\n");

    // Variables de la tienda.
    int girasolComprado = 0;
    int filaPlanta = -1;
    int columnaPlanta = -1;
    int seguirComprando = 1;

    while (seguirComprando == 1) {

        printf("Entrando a la tienda\n");

        comprarPlanta(
            partida.jardin,
            filas,
            columnas,
            &partida.soles,
            &girasolComprado,
            &filaPlanta,
            &columnaPlanta
        );

        printf("\nQuieres comprar otra planta? (1:SI/0:NO): ");

        if (scanf("%d", &seguirComprando) != 1) {
            seguirComprando = 0;
        }
    }

    printf("Sali de la tienda correctamente\n");

    // Generamos la primera oleada.
    generarOleadas(
        partida.jardin,
        partida.zombies,
        &partida.cantidadZombies,
        &partida.zombiesGenerados,
        partida.zombiesPorOleada,
        partida.capacidadZombies,
        filas,
        columnas,
        config.zombies,
        config.cantidadZombies
    );

    float tiempoAnterior = GetTime();

    printf("Comenzando el juego\n");

    // Bucle principal.
    while (!WindowShouldClose()) {

        // Actualizamos el juego cada segundo.
        if (GetTime() - tiempoAnterior >= 1.0f &&
            partida.juegoTerminado == 0 && entreOleadas ==0) {

            printf("\nTurno %d:\n", partida.turno);

            // Actualizamos el turno.
            actualizarTurno(
                partida.jardin,
                partida.zombies,
                partida.cantidadZombies,
                &partida.soles,
                &partida.contadorGirasoles,
                &partida.juegoTerminado,
                partida.cortacesped,
                filas,
                columnas,
                contadoresAtaque
            );

            // Generamos un zombie si queda alguno pendiente.
            if (partida.zombiesGenerados < partida.zombiesPorOleada) {

                generarOleadas(
                    partida.jardin,
                    partida.zombies,
                    &partida.cantidadZombies,
                    &partida.zombiesGenerados,
                    partida.zombiesPorOleada,
                    partida.capacidadZombies,
                    filas,
                    columnas,
                    config.zombies,
                    config.cantidadZombies
                );
            }

            // Contamos los zombies que siguen activos.
            int zombiesActivos = 0;

            for (int i = 0; i < partida.cantidadZombies; i++) {

                if (partida.zombies[i].activo == 1) {
                    zombiesActivos++;
                }
            }

            printf("Zombies activos: %d\n", zombiesActivos);

            // Comprobamos si termino la oleada.
        if (zombiesActivos == 0 &&
            partida.zombiesGenerados == partida.zombiesPorOleada &&
            partida.juegoTerminado == 0) {

         if (partida.oleadaActual < config.cantidadOleadas) {

        // Comenzamos el entretiempo.
        entreOleadas = 1;
        tiempoInicipEntreOleadas = GetTime();

        printf("Oleada completada. Descanso de 10 segundos.\n");

    } else {

        printf("FELICIDADES! GANASTE EL JUEGO\n");
        partida.juegoTerminado = 1;
    }
}

            // Imprimimos la matriz actualizada.
            imprimirMatriz(
                partida.jardin,
                &filas,
                &columnas
            );

            // Actualizamos el tiempo y el turno.
            tiempoAnterior = GetTime();
            partida.turno++;
        }
        // Comprobamos si termino el entretiempo.
if (entreOleadas == 1 &&
    GetTime() - tiempoInicipEntreOleadas >= 10.0f) {

    entreOleadas = 0;

    partida.oleadaActual++;

    partida.zombiesPorOleada += config.incrementoZombies;

    // Reiniciamos el contador para la nueva oleada.
    partida.cantidadZombies = 0;
    partida.zombiesGenerados = 0;

    printf("Comienza la oleada %d con %d zombies\n",
           partida.oleadaActual,
           partida.zombiesPorOleada);

    tiempoAnterior = GetTime();
}
        // Dibujamos el jardin en cada fotograma.
        BeginDrawing();

        ClearBackground(RAYWHITE);

        dibujarJardin(
            partida.jardin,
            &filas,
            &columnas,
            partida.zombies,
            &partida.cantidadZombies
        );
        // Mostramos el entretiempo entre oleadas.
if (entreOleadas == 1) {

    int segundosRestantes =
        10 - (int)(GetTime() - tiempoInicipEntreOleadas);

    if (segundosRestantes < 0) {
        segundosRestantes = 0;
    }

    DrawText(
        "OLEADA COMPLETADA",
        300, 30, 25, DARKBLUE
    );

    DrawText(
        TextFormat("Siguiente oleada en: %d", segundosRestantes),
        300, 65, 20, DARKGRAY
    );
}

        EndDrawing();
    }

    // Cerramos la ventana del juego.
    CloseWindow();

    // Liberamos la memoria dinamica.
    free(partida.cortacesped);
    free(partida.zombies);

    liberarJardin(contadoresAtaque, &filas);
    liberarJardin(partida.jardin, &filas);
    liberarConfiguracion(&config);

    return 0;
}
