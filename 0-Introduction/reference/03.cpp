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
        const Vector2 mouse_position = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            std::cout << "Mouse left button pressed at ("<< mouse_position.x << ", "<< mouse_position.y << ")\n";

        BeginDrawing();

        ClearBackground(Color{24, 28, 36, 255});
        DrawLine(80, 80, 420, 200, Color{255, 196, 61, 255});
        DrawCircleV(mouse_position, 70.0f, Color{72, 187, 120, 255});
        DrawRectangle(880, 140, 180, 180, Color{99, 179, 237, 255});

        EndDrawing();
    }
      
    CloseWindow();
    
    return 0;
}
