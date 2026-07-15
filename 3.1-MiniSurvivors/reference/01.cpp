#include <raylib.h>

static constexpr int window_width = 960;
static constexpr int window_height = 540;
static constexpr float player_speed = 260.0f;

float clamp_value(float value, float minimum, float maximum)
{
    if (value < minimum) return minimum;
    if (value > maximum) return maximum;
    return value;
}

int main()
{
    InitWindow(window_width, window_height, "MiniSurvivors - 01 Player");
    SetTargetFPS(60);

    Vector2 player_position{
        window_width / 2.0f,
        window_height / 2.0f,
    };

    while (!WindowShouldClose())
    {
        const float delta_time = GetFrameTime();

        // input
        if (IsKeyDown(KEY_W)) player_position.y -= player_speed * delta_time;
        if (IsKeyDown(KEY_S)) player_position.y += player_speed * delta_time;
        if (IsKeyDown(KEY_A)) player_position.x -= player_speed * delta_time;
        if (IsKeyDown(KEY_D)) player_position.x += player_speed * delta_time;

        // update
        player_position.x = clamp_value(player_position.x, 20.0f, window_width - 20.0f);
        player_position.y = clamp_value(player_position.y, 20.0f, window_height - 20.0f);

        // render
        BeginDrawing();
        ClearBackground(Color{24, 28, 36, 255});
        DrawCircleV(player_position, 20.0f, GREEN);
        DrawText("W A S D to move", 20, 20, 20, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
