#include <cmath>
#include <vector>

#include <raylib.h>

static constexpr int window_width = 960;
static constexpr int window_height = 540;
static constexpr float player_speed = 260.0f;
static constexpr float enemy_speed = 70.0f;

float clamp_value(float value, float minimum, float maximum)
{
    if (value < minimum) return minimum;
    if (value > maximum) return maximum;
    return value;
}

struct Enemy
{
    Vector2 position;
};

Vector2 direction_to(Vector2 from, Vector2 to)
{
    Vector2 direction{
        to.x - from.x,
        to.y - from.y,
    };
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length == 0.0f) return Vector2{0.0f, 0.0f};
    return Vector2{direction.x / length, direction.y / length};
}

Enemy create_enemy()
{
    const int edge = GetRandomValue(0, 3);
    if (edge == 0) return Enemy{Vector2{static_cast<float>(GetRandomValue(0, window_width)), -20.0f}};
    if (edge == 1) return Enemy{Vector2{static_cast<float>(GetRandomValue(0, window_width)), window_height + 20.0f}};
    if (edge == 2) return Enemy{Vector2{-20.0f, static_cast<float>(GetRandomValue(0, window_height))}};
    return Enemy{Vector2{window_width + 20.0f, static_cast<float>(GetRandomValue(0, window_height))}};
}

int main()
{
    InitWindow(window_width, window_height, "MiniSurvivors - 02 Enemies");
    SetTargetFPS(60);

    Vector2 player_position{window_width / 2.0f, window_height / 2.0f};
    std::vector<Enemy> enemies;
    float spawn_timer = 0.0f;

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

        spawn_timer += delta_time;
        if (spawn_timer >= 1.0f)
        {
            enemies.push_back(create_enemy());
            spawn_timer = 0.0f;
        }

        for (Enemy& enemy : enemies)
        {
            const Vector2 direction = direction_to(enemy.position, player_position);
            enemy.position.x += direction.x * enemy_speed * delta_time;
            enemy.position.y += direction.y * enemy_speed * delta_time;
        }

        // render
        BeginDrawing();
        ClearBackground(Color{24, 28, 36, 255});
        DrawCircleV(player_position, 20.0f, GREEN);
        for (const Enemy& enemy : enemies)
        {
            DrawCircleV(enemy.position, 16.0f, RED);
        }
        DrawText("W A S D to move", 20, 20, 20, RAYWHITE);
        DrawText(TextFormat("Enemies: %d", static_cast<int>(enemies.size())), 20, 48, 20, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
