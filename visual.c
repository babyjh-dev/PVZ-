#include "raylib.h"

void mainVisual(){

    // creamos la ventana
    InitWindow(900, 600, "Plants vs Zombies");

    // establecemos los FPS
    SetTargetFPS(60);

    // mantenemos la ventana abierta
    while(!WindowShouldClose()){

        BeginDrawing();

        // pintamos el fondo
        ClearBackground(RAYWHITE);

        // mostramos el titulo
        DrawText("PLANTS VS ZOMBIES", 300, 30, 30, DARKGREEN);

        EndDrawing();
    }

    // cerramos la ventana
    CloseWindow();

}
