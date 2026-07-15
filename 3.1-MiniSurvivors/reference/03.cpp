#include <cmath>
#include <vector>

#include <raylib.h>

static constexpr int window_width = 960;
static constexpr int window_height = 540;
static constexpr float player_speed = 260.0f;
static constexpr float enemy_speed = 70.0f;
static constexpr float bullet_radius = 7.0f;
static constexpr float bullet_orbit_radius = 90.0f;
static constexpr float bullet_rotation_speed = 2.5f;

float clamp_value(float value, float minimum, float maximum)
{
    if (value < minimum) return minimum;
    if (value > maximum) return maximum;
    return value;
}

struct Enemy { Vector2 position; };
struct Bullet { float angle; };

Vector2 direction_to(Vector2 from, Vector2 to)
{
    Vector2 direction{to.x - from.x, to.y - from.y};
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

Vector2 bullet_position(Vector2 player_position, const Bullet& bullet)
{
    return Vector2{
        player_position.x + std::cos(bullet.angle) * bullet_orbit_radius,
        player_position.y + std::sin(bullet.angle) * bullet_orbit_radius,
    };
}

void reset_game(Vector2& player_position, std::vector<Enemy>& enemies, int& score, bool& game_over)
{
    player_position = Vector2{window_width / 2.0f, window_height / 2.0f};
    enemies.clear();
    score = 0;
    game_over = false;
}

int main()
{
    InitWindow(window_width, window_height, "MiniSurvivors - 03 Combat");
    SetTargetFPS(60);

    Vector2 player_position{window_width / 2.0f, window_height / 2.0f};
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets{{0.0f}, {PI / 2.0f}, {PI}, {PI * 1.5f}};
    float spawn_timer = 0.0f;
    int score = 0;
    bool game_over = false;

    while (!WindowShouldClose())
    {
        const float delta_time = GetFrameTime();

        // input
        if (!game_over)
        {
            if (IsKeyDown(KEY_W)) player_position.y -= player_speed * delta_time;
            if (IsKeyDown(KEY_S)) player_position.y += player_speed * delta_time;
            if (IsKeyDown(KEY_A)) player_position.x -= player_speed * delta_time;
            if (IsKeyDown(KEY_D)) player_position.x += player_speed * delta_time;
        }
        if (game_over && IsKeyPressed(KEY_R))
        {
            reset_game(player_position, enemies, score, game_over);
        }

        // update
        if (!game_over)
        {
            player_position.x = clamp_value(player_position.x, 20.0f, window_width - 20.0f);
            player_position.y = clamp_value(player_position.y, 20.0f, window_height - 20.0f);

            spawn_timer += delta_time;
            if (spawn_timer >= 1.0f)
            {
                enemies.push_back(create_enemy());
                spawn_timer = 0.0f;
            }

            for (Bullet& bullet : bullets)
            {
                bullet.angle += bullet_rotation_speed * delta_time;
            }

            for (Enemy& enemy : enemies)
            {
                const Vector2 direction = direction_to(enemy.position, player_position);
                enemy.position.x += direction.x * enemy_speed * delta_time;
                enemy.position.y += direction.y * enemy_speed * delta_time;
                if (CheckCollisionCircles(enemy.position, 16.0f, player_position, 20.0f))
                {
                    game_over = true;
                }
            }

            for (size_t enemy_index = 0; enemy_index < enemies.size();)
            {
                bool enemy_hit = false;
                for (const Bullet& bullet : bullets)
                {
                    if (CheckCollisionCircles(bullet_position(player_position, bullet), bullet_radius, enemies[enemy_index].position, 16.0f))
                    {
                        enemy_hit = true;
                        break;
                    }
                }
                if (enemy_hit)
                {
                    enemies.erase(enemies.begin() + enemy_index);
                    ++score;
                }
                else
                {
                    ++enemy_index;
                }
            }
        }

        // render
        BeginDrawing();
        ClearBackground(Color{24, 28, 36, 255});
        DrawCircleV(player_position, 20.0f, GREEN);
        for (const Bullet& bullet : bullets)
        {
            DrawCircleV(bullet_position(player_position, bullet), bullet_radius, YELLOW);
        }
        for (const Enemy& enemy : enemies)
        {
            DrawCircleV(enemy.position, 16.0f, RED);
        }
        DrawText(TextFormat("Score: %d", score), 20, 20, 20, RAYWHITE);
        if (game_over)
        {
            DrawText("GAME OVER", 350, 230, 40, RAYWHITE);
            DrawText("Press R to restart", 350, 280, 20, RAYWHITE);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
