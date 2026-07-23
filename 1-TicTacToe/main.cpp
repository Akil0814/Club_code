#include <raylib.h>

const int window_width = 900;
const int window_height = 900;


int main()
{
    InitWindow(window_width, window_height, "Tic Tac Toe");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}