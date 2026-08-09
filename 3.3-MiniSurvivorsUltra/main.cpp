#include <raylib.h>

static constexpr int window_width = 1280;
static constexpr int window_height = 720;

int main()
{
    InitWindow(window_width, window_height, "MiniSurvivors");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const float delta_time = GetFrameTime();

        // input

        // update

        // render
        BeginDrawing();
        ClearBackground(Color{24, 28, 36, 255});
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
