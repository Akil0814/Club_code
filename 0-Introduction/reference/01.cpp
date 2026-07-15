#include <iostream>

#include <raylib.h>

int main()
{
    int window_width = 1280;
    int window_height = 720;

    InitWindow(window_width, window_height, "Raylib Template");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        std::cout<<"hello Raylib"<<std::endl;

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
