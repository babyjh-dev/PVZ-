#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <time.h>

#include "configuracion.h"
#include "jardin.h"
#include "plantas.h"
#include "zombies.h"
#include "juego.h"

// Funcion principal del programa
int main(void) {

    // Inicializamos la semilla para generar valores aleatorios
    srand(time(NULL));

    // Cargamos la configuracion del juego desde el archivo
    Configuracion config = {0};

    if (!cargarConfiguracion(&config, "configuracion.txt")) {
        printf("Error al cargar la configuracion\n");
        return 1;
    }

    // Creamos e inicializamos la estructura de la partida
    Partida partida = {0};

    // Cargamos el jardin utilizando memoria dinamica
    partida.jardin = leerJardin("jardin.txt", &partida.filas, &partida.columnas);

    // Comprobamos si el jardin se cargo correctamente
    if (partida.jardin == NULL) {
        printf("Error al cargar el jardin\n");
        liberarConfiguracion(&config);
        return 1;
    }

    int filas = partida.filas;
    int columnas = partida.columnas;

    // Creamos una matriz para llevar los contadores de ataque
    int **contadoresAtaque = crearJardin(&filas, &columnas);

    // Comprobamos si se pudo crear la matriz
    if (contadoresAtaque == NULL) {
        printf("Error al crear los contadores de ataque\n");
        liberarJardin(partida.jardin, &filas);
        liberarConfiguracion(&config);
        return 1;
    }

    // Calculamos la capacidad necesaria para almacenar los zombies de todas las oleadas
    int capacidadZombies = config.zombiesPrimeraOleada + (config.cantidadOleadas - 1) * config.incrementoZombies;

    if (capacidadZombies <= 0) {
        printf("Error: capacidad de zombies no valida\n");
        liberarJardin(contadoresAtaque, &filas);
        liberarJardin(partida.jardin, &filas);
        liberarConfiguracion(&config);
        return 1;
    }

    // Reservamos memoria dinamica para almacenar los zombies
    partida.capacidadZombies = capacidadZombies;
    partida.zombies = malloc(partida.capacidadZombies * sizeof(Zombie));

    if (partida.zombies == NULL) {
        printf("Error al reservar memoria para los zombies\n");
        liberarJardin(contadoresAtaque, &filas);
        liberarJardin(partida.jardin, &filas);
        liberarConfiguracion(&config);
        return 1;
    }

    // Reservamos memoria para los cortacespedes, uno por cada fila
    partida.cortacesped = malloc(filas * sizeof(int));

    if (partida.cortacesped == NULL) {
        printf("Error al reservar memoria para los cortacespedes\n");
        free(partida.zombies);
        liberarJardin(contadoresAtaque, &filas);
        liberarJardin(partida.jardin, &filas);
        liberarConfiguracion(&config);
        return 1;
    }

    // Inicializamos todos los cortacespedes como disponibles
    for (int i = 0; i < filas; i++) {
        partida.cortacesped[i] = 1;
    }

    // Reservamos memoria para almacenar los soles que aparecen en el jardin
    partida.capacidadSoles = 100;
    partida.solesJardin = malloc(partida.capacidadSoles * sizeof(Sol));

    if (partida.solesJardin == NULL) {
        printf("error al reservar memoria para los soles\n");
        free(partida.cortacesped);
        free(partida.zombies);
        liberarJardin(contadoresAtaque, &filas);
        liberarJardin(partida.jardin, &filas);
        liberarConfiguracion(&config);
        return 1;
    }

    // Inicializamos la cantidad de soles que hay en el jardin
    partida.cantidadSoles = 0;

    // Inicializamos los valores iniciales de la partida
    partida.soles = config.solesIniciales;
    partida.oleadaActual = 1;
    partida.zombiesPorOleada = config.zombiesPrimeraOleada;
    partida.cantidadZombies = 0;
    partida.zombiesGenerados = 0;

    partida.turno = 1;
    partida.contadorGirasoles = 0;
    partida.juegoTerminado = 0;

    // Variables utilizadas para controlar el descanso entre oleadas
    int entreOleadas = 0;
    float tiempoInicipEntreOleadas = 0;

    // Abrimos la ventana donde se mostrara el juego
    InitWindow(900, 600, "Plants vs Zombies");
    SetTargetFPS(60);

    printf("El juego comenzo\n");

    // Inicializamos las variables utilizadas en la tienda
    int girasolComprado = 0;
    int filaPlanta = -1;
    int columnaPlanta = -1;
    int seguirComprando = 1;

    // Permitimos al jugador comprar plantas antes de comenzar la partida
    while (seguirComprando == 1) {

        printf("Entrando a la tienda\n");

        comprarPlanta(
            partida.jardin,
            &filas,
            &columnas,
            &partida.soles,
            &girasolComprado,
            &filaPlanta,
            &columnaPlanta
        );

        printf("\nQuieres comprar otra planta? (1:SI/0:NO): ");

        // Leemos si el jugador desea seguir comprando
        if (scanf("%d", &seguirComprando) != 1) {
            seguirComprando = 0;
        }
    }

    printf("Sali de la tienda correctamente\n");

    // Generamos los primeros zombies de la partida
    generarOleadas(
        partida.jardin,
        partida.zombies,
        &partida.cantidadZombies,
        &partida.zombiesGenerados,
        &partida.zombiesPorOleada,
        &partida.capacidadZombies,
        &filas,
        &columnas,
        config.zombies,
        &config.cantidadZombies
    );

    float tiempoAnterior = GetTime();

    printf("Comenzando el juego\n");

    // Bucle principal que mantiene el juego en ejecucion
    while (!WindowShouldClose()) {

        // Actualizamos la logica del juego cada segundo
        if (GetTime() - tiempoAnterior >= 1.0f && partida.juegoTerminado == 0 && entreOleadas == 0) {

            printf("\nTurno %d:\n", partida.turno);

            // Actualizamos el estado de las plantas, zombies y soles
            actualizarTurno(
                partida.jardin,
                partida.zombies,
                &partida.cantidadZombies,
                &partida.soles, // Dinero disponible del jugador
                &partida.contadorGirasoles,
                &partida.juegoTerminado,
                partida.cortacesped,
                &filas,
                &columnas,
                contadoresAtaque,
                partida.solesJardin,
                &partida.cantidadSoles,
                &partida.capacidadSoles
            );

            // Generamos un zombie si todavia quedan zombies pendientes
            if (partida.zombiesGenerados < partida.zombiesPorOleada) {

                generarOleadas(
                    partida.jardin,
                    partida.zombies,
                    &partida.cantidadZombies,
                    &partida.zombiesGenerados,
                    &partida.zombiesPorOleada,
                    &partida.capacidadZombies,
                    &filas,
                    &columnas,
                    config.zombies,
                    &config.cantidadZombies
                );
            }

            // Contamos la cantidad de zombies que siguen activos
            int zombiesActivos = 0;

            for (int i = 0; i < partida.cantidadZombies; i++) {
                if (partida.zombies[i].activo == 1) {
                    zombiesActivos++;
                }
            }

            printf("Zombies activos: %d\n", zombiesActivos);

            // Comprobamos si todos los zombies de la oleada fueron derrotados
            if (zombiesActivos == 0 && partida.zombiesGenerados == partida.zombiesPorOleada && partida.juegoTerminado == 0) {

                // Si quedan oleadas pendientes, iniciamos el descanso
                if (partida.oleadaActual < config.cantidadOleadas) {

                    entreOleadas = 1;
                    tiempoInicipEntreOleadas = GetTime();

                    printf("Oleada completada. Descanso de 10 segundos.\n");

                } else {

                    // Si no quedan oleadas, el jugador gana la partida
                    printf("FELICIDADES! GANASTE EL JUEGO\n");
                    partida.juegoTerminado = 1;
                }
            }

            // Mostramos la matriz actualizada en la consola
            imprimirMatriz(
                partida.jardin,
                &filas,
                &columnas
            );

            // Actualizamos el tiempo y avanzamos al siguiente turno
            tiempoAnterior = GetTime();
            partida.turno++;
        }

        // Comprobamos si ya termino el descanso entre oleadas
        if (entreOleadas == 1 &&
            GetTime() - tiempoInicipEntreOleadas >= 10.0f) {

            entreOleadas = 0;

            // Avanzamos a la siguiente oleada y aumentamos su cantidad de zombies
            partida.oleadaActual++;
            partida.zombiesPorOleada += config.incrementoZombies;

            // Reiniciamos los contadores de zombies para la nueva oleada
            partida.cantidadZombies = 0;
            partida.zombiesGenerados = 0;

            printf("Comienza la oleada %d con %d zombies\n",
                partida.oleadaActual,
                partida.zombiesPorOleada);

            tiempoAnterior = GetTime();
        }

        // Revisamos si el jugador presiona el boton izquierdo del mouse para recoger un sol
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

            Vector2 mouse = GetMousePosition();

            // Revisamos todos los soles que aparecen en el jardin
            for (int i = 0; i < partida.cantidadSoles; i++) {

                // Comprobamos si el sol esta activo y si el mouse esta sobre el
                if (partida.solesJardin[i].activo == 1 &&
                    CheckCollisionPointCircle(
                        mouse,
                        (Vector2){
                            partida.solesJardin[i].x,
                            partida.solesJardin[i].y
                        },
                        18
                    )) {

                    // Sumamos el valor del sol al dinero disponible
                    partida.soles += partida.solesJardin[i].valor;

                    // Desactivamos el sol para que desaparezca del jardin
                    partida.solesJardin[i].activo = 0;

                    printf("Sol recogido. Soles disponibles: %d\n", partida.soles);
                }
            }
        }

        // Comenzamos a dibujar los elementos de la ventana
        BeginDrawing();

        // Limpiamos el fondo antes de dibujar el nuevo fotograma
        ClearBackground(RAYWHITE);

        // Dibujamos el jardin y los elementos que se encuentran en el
        dibujarJardin(
            partida.jardin,
            &filas,
            &columnas,
            partida.zombies,
            &partida.cantidadZombies
        );

        // Dibujamos los soles que se encuentran activos en el jardin
        for (int i = 0; i < partida.cantidadSoles; i++) {
            if (partida.solesJardin[i].activo == 1) {
                DrawCircle(
                    partida.solesJardin[i].x,
                    partida.solesJardin[i].y,
                    15,
                    GOLD
                );

                DrawCircle(
                    partida.solesJardin[i].x,
                    partida.solesJardin[i].y,
                    10,
                    YELLOW
                );
            }
        }

        // Mostramos en pantalla el tiempo restante para la siguiente oleada
        if (entreOleadas == 1) {

            int segundosRestantes = 10 - (int)(GetTime() - tiempoInicipEntreOleadas);

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

        // Finalizamos el dibujo del fotograma actual
        EndDrawing();
    }

    // Cerramos la ventana al finalizar el juego
    CloseWindow();

    // Liberamos toda la memoria dinamica reservada durante la partida
    free(partida.cortacesped);
    free(partida.zombies);
    free(partida.solesJardin);
    liberarJardin(contadoresAtaque, &filas);
    liberarJardin(partida.jardin, &filas);
    liberarConfiguracion(&config);

    return 0;
}
