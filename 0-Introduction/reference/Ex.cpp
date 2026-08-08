#include <iostream>

#include <raylib.h>

int main()
{
    const int window_width = 1280;
    const int window_height = 720;
    const int font_size = 30;
    const char* FollowText = "Raylib";

    InitWindow(window_width, window_height, "Raylib Template");
    SetTargetFPS(60);
    const int text_width = MeasureText(FollowText, font_size);
    const int text_x = (window_width - text_width) / 2;
    const int text_y = (window_height - font_size) / 2;

    while (!WindowShouldClose())
    {

        const Vector2 mouse_position = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            std::cout << "Mouse left button pressed at ("<< mouse_position.x << ", "<< mouse_position.y << ")\n";

        BeginDrawing();

        ClearBackground(BLACK);
        DrawLine(80, 80, 420, 200, GOLD);
        DrawCircleV(mouse_position, 70.0f, GREEN);
        DrawRectangle(880, 140, 180, 180, SKYBLUE);
        DrawText(FollowText, text_x, text_y, font_size, RAYWHITE);

        EndDrawing();
    }
      
    CloseWindow();
    
    return 0;
}
