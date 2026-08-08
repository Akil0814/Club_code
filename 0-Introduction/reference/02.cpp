#include <iostream>

#include <raylib.h>

int main()
{
    const int window_width = 1280;
    const int window_height = 720;

    InitWindow(window_width, window_height, "Raylib Template");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        BeginDrawing();

        ClearBackground(BLACK);

        DrawLine(80, 80, 420, 200, GOLD);
        DrawCircleV(Vector2{180,120}, 70.0f, GREEN);
        DrawRectangle(880, 140, 180, 180, SKYBLUE);

        EndDrawing();
    }
      
    CloseWindow();
    
    return 0;
}
