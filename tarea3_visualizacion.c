
#include <raylib.h>
#include "tarea3_visualizacion.h"

#define TAM_CASILLA 70
#define INICIO_X 100
#define INICIO_Y 130

void dibujarJInterfaz(int** jardin, int filas, int columnas,
                   int soles, int vidaZombie, int juegoTerminado,
                   int plantaSeleccionada){

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Titulo
    DrawText("PLANTS VS ZOMBIES", 280, 20, 30, DARKGREEN);

    // Informacion del jugador
    DrawText(TextFormat("Soles: %d", soles),
             100, 65, 25, GOLD);

    DrawText("1: Girasol (50)", 350, 65, 18, DARKGREEN);
    DrawText("2: Lanzaguisantes (100)", 500, 65, 18, DARKGREEN);
    DrawText("3: Nuez (150)", 730, 65, 18, DARKGREEN);

    // Planta seleccionada
    int numeroSeleccionado = 0;

    if(plantaSeleccionada == 2){
        numeroSeleccionado = 1;
    }
    else if(plantaSeleccionada == 1){
        numeroSeleccionado = 2;
    }
    else if(plantaSeleccionada == 3){
        numeroSeleccionado = 3;
    }

    if(numeroSeleccionado != 0){
        DrawText(
            TextFormat("Planta seleccionada: %d", numeroSeleccionado),
            100, 100, 18, DARKGRAY
        );
    }

    // Dibujar jardin
    for(int i = 0; i < filas; i++){
        for(int j = 0; j < columnas; j++){

            int x = INICIO_X + j * TAM_CASILLA;
            int y = INICIO_Y + i * TAM_CASILLA;

            DrawRectangle(x, y, TAM_CASILLA, TAM_CASILLA, LIME);
            DrawRectangleLines(x, y, TAM_CASILLA, TAM_CASILLA, DARKGREEN);

            // Girasol
            if(jardin[i][j] == 2){
                DrawCircle(x+35, y+35, 23, YELLOW);
                DrawCircle(x+35, y+35, 10, ORANGE);
            }

            // Lanzaguisantes
            else if(jardin[i][j] == 1){
                DrawCircle(x+35, y+35, 22, DARKGREEN);
                DrawCircle(x+52, y+30, 9, GREEN);
            }

            // Nuez
            else if(jardin[i][j] == 3){
                DrawEllipse(x+35, y+38, 23, 27, BROWN);
            }

            // Zombie
            else if(jardin[i][j] == 4){
                DrawCircle(x+35, y+35, 23, RED);
                DrawCircle(x+27, y+28, 4, WHITE);
                DrawCircle(x+43, y+28, 4, WHITE);
            }
        }
    }

    // Informacion del zombie
    if(!juegoTerminado){
        DrawText(
            TextFormat("Vida del zombie: %d", vidaZombie),
            100, 510, 20, RED
        );
    }

    // Mensajes de finalizacion
    if(juegoTerminado){
        if(vidaZombie <= 0){
            DrawText("GANASTE! Zombie derrotado",
                     250, 530, 25, DARKGREEN);
        }
        else{
            DrawText("PERDISTE! La casa fue atacada",
                     250, 530, 25, RED);
        }
    }

    EndDrawing();
}


