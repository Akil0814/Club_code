#include <array>
#include <cmath>
#include <vector>

#include <raylib.h>

static constexpr int window_width = 960;
static constexpr int window_height = 540;
static constexpr int animation_frame_count = 6;
static constexpr float player_speed = 260.0f;
static constexpr float enemy_speed = 70.0f;
static constexpr float bullet_radius = 7.0f;
static constexpr float bullet_orbit_radius = 58.0f;
static constexpr float bullet_rotation_speed = 2.5f;

float clamp_value(float value, float minimum, float maximum)
{
    if (value < minimum) return minimum;
    if (value > maximum) return maximum;
    return value;
}

struct Animation
{
    std::array<Texture2D, animation_frame_count> frames{};
    int current_frame = 0;
    float timer = 0.0f;

    void update(float delta_time)
    {
        timer += delta_time;
        if (timer >= 0.10f)
        {
            timer = 0.0f;
            current_frame = (current_frame + 1) % animation_frame_count;
        }
    }

    void unload()
    {
        for (Texture2D frame : frames) UnloadTexture(frame);
    }
};

struct ActorAnimation
{
    Animation left;
    Animation right;
    bool facing_left = false;
};

struct Enemy { Vector2 position; ActorAnimation animation; };
struct Bullet { float angle; };

Vector2 direction_to(Vector2 from, Vector2 to)
{
    Vector2 direction{to.x - from.x, to.y - from.y};
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length == 0.0f) return Vector2{0.0f, 0.0f};
    return Vector2{direction.x / length, direction.y / length};
}

Animation load_animation(const char* path_pattern)
{
    Animation animation;
    for (int frame = 0; frame < animation_frame_count; ++frame)
    {
        animation.frames[frame] = LoadTexture(TextFormat(path_pattern, frame));
    }
    return animation;
}

Enemy create_enemy()
{
    const int edge = GetRandomValue(0, 3);
    Vector2 position{};
    if (edge == 0) position = Vector2{static_cast<float>(GetRandomValue(0, window_width)), -32.0f};
    if (edge == 1) position = Vector2{static_cast<float>(GetRandomValue(0, window_width)), window_height + 32.0f};
    if (edge == 2) position = Vector2{-32.0f, static_cast<float>(GetRandomValue(0, window_height))};
    if (edge == 3) position = Vector2{window_width + 32.0f, static_cast<float>(GetRandomValue(0, window_height))};

    Enemy enemy{position};
    enemy.animation.left = load_animation("res/enemy_left_%d.png");
    enemy.animation.right = load_animation("res/enemy_right_%d.png");
    return enemy;
}

Vector2 bullet_position(Vector2 player_position, const Bullet& bullet)
{
    return Vector2{
        player_position.x + std::cos(bullet.angle) * bullet_orbit_radius,
        player_position.y + std::sin(bullet.angle) * bullet_orbit_radius,
    };
}

void draw_animation(const Animation& animation, Vector2 center)
{
    const Texture2D texture = animation.frames[animation.current_frame];
    DrawTexture(
        texture,
        static_cast<int>(center.x - texture.width / 2.0f),
        static_cast<int>(center.y - texture.height / 2.0f),
        WHITE);
}

int main()
{
    InitWindow(window_width, window_height, "MiniSurvivors - 04 Assets");
    InitAudioDevice();
    SetTargetFPS(60);

    ActorAnimation player_animation;
    player_animation.left = load_animation("res/player_left_%d.png");
    player_animation.right = load_animation("res/player_right_%d.png");
    Texture2D player_shadow = LoadTexture("res/shadow_player.png");
    Texture2D enemy_shadow = LoadTexture("res/shadow_enemy.png");
    Sound hit_sound = LoadSound("res/hit.wav");

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
            const Vector2 old_position = player_position;
            if (IsKeyDown(KEY_W)) player_position.y -= player_speed * delta_time;
            if (IsKeyDown(KEY_S)) player_position.y += player_speed * delta_time;
            if (IsKeyDown(KEY_A)) player_position.x -= player_speed * delta_time;
            if (IsKeyDown(KEY_D)) player_position.x += player_speed * delta_time;
            if (player_position.x < old_position.x) player_animation.facing_left = true;
            if (player_position.x > old_position.x) player_animation.facing_left = false;
        }
        if (game_over && IsKeyPressed(KEY_R))
        {
            player_position = Vector2{window_width / 2.0f, window_height / 2.0f};
            enemies.clear();
            score = 0;
            game_over = false;
        }

        // update
        if (!game_over)
        {
            player_position.x = clamp_value(player_position.x, 32.0f, window_width - 32.0f);
            player_position.y = clamp_value(player_position.y, 32.0f, window_height - 32.0f);
            player_animation.left.update(delta_time);
            player_animation.right.update(delta_time);

            spawn_timer += delta_time;
            if (spawn_timer >= 1.0f)
            {
                enemies.push_back(create_enemy());
                spawn_timer = 0.0f;
            }

            for (Bullet& bullet : bullets) bullet.angle += bullet_rotation_speed * delta_time;
            for (Enemy& enemy : enemies)
            {
                const Vector2 direction = direction_to(enemy.position, player_position);
                enemy.position.x += direction.x * enemy_speed * delta_time;
                enemy.position.y += direction.y * enemy_speed * delta_time;
                enemy.animation.facing_left = direction.x < 0.0f;
                enemy.animation.left.update(delta_time);
                enemy.animation.right.update(delta_time);
                if (CheckCollisionCircles(enemy.position, 24.0f, player_position, 24.0f)) game_over = true;
            }

            for (size_t enemy_index = 0; enemy_index < enemies.size();)
            {
                bool enemy_hit = false;
                for (const Bullet& bullet : bullets)
                {
                    if (CheckCollisionCircles(bullet_position(player_position, bullet), bullet_radius, enemies[enemy_index].position, 24.0f))
                    {
                        enemy_hit = true;
                        break;
                    }
                }
                if (enemy_hit)
                {
                    PlaySound(hit_sound);
                    enemies[enemy_index].animation.left.unload();
                    enemies[enemy_index].animation.right.unload();
                    enemies.erase(enemies.begin() + enemy_index);
                    ++score;
                }
                else ++enemy_index;
            }
        }

        // render
        BeginDrawing();
        ClearBackground(Color{24, 28, 36, 255});
        DrawTexture(player_shadow, static_cast<int>(player_position.x - player_shadow.width / 2.0f), static_cast<int>(player_position.y + 20.0f), WHITE);
        draw_animation(player_animation.facing_left ? player_animation.left : player_animation.right, player_position);
        for (const Bullet& bullet : bullets) DrawCircleV(bullet_position(player_position, bullet), bullet_radius, YELLOW);
        for (const Enemy& enemy : enemies)
        {
            DrawTexture(enemy_shadow, static_cast<int>(enemy.position.x - enemy_shadow.width / 2.0f), static_cast<int>(enemy.position.y + 20.0f), WHITE);
            draw_animation(enemy.animation.facing_left ? enemy.animation.left : enemy.animation.right, enemy.position);
        }
        DrawText(TextFormat("Score: %d", score), 20, 20, 20, RAYWHITE);
        if (game_over)
        {
            DrawText("GAME OVER", 350, 230, 40, RAYWHITE);
            DrawText("Press R to restart", 350, 280, 20, RAYWHITE);
        }
        EndDrawing();
    }

    player_animation.left.unload();
    player_animation.right.unload();
    for (Enemy& enemy : enemies)
    {
        enemy.animation.left.unload();
        enemy.animation.right.unload();
    }
    UnloadTexture(player_shadow);
    UnloadTexture(enemy_shadow);
    UnloadSound(hit_sound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
