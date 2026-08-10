#include <raylib.h>

#include "game_config.h"

int main()
{
    InitWindow(window_width, window_height, "MiniSurvivors Pro - Interface Scaffold");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

