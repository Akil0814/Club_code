#include <iostream>

#include <raylib.h>

int main(int argc, char** argv)
{
    static constexpr int window_width = 1280;
    static constexpr int window_height = 720;
    static constexpr int font_size = 30;
    const char* FollowText = "Raylib";

    InitWindow(window_width, window_height, "Raylib Template");
    SetTargetFPS(60);
    const int text_width = MeasureText(FollowText, font_size);
    const int text_x = (window_width - text_width) / 2;
    const int text_y = (window_height - font_size) / 2;

    while (!WindowShouldClose())
    {
        //update
        const Vector2 mouse_position = GetMousePosition();

        //input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            std::cout << "Mouse left button pressed at ("
                      << mouse_position.x << ", "
                      << mouse_position.y << ")\n";
        }

        BeginDrawing();

        //render
        ClearBackground(Color{24, 28, 36, 255});
        DrawLine(80, 80, 420, 200, Color{255, 196, 61, 255});
        DrawCircleV(mouse_position, 70.0f, Color{72, 187, 120, 255});
        DrawRectangle(880, 140, 180, 180, Color{99, 179, 237, 255});
        DrawText(FollowText, text_x, text_y, font_size, RAYWHITE);

        EndDrawing();
    }
      
    CloseWindow();
    
    return 0;
}
