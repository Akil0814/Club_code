#include <cmath>
#include <vector>

#include <raylib.h>

static constexpr int window_width = 1280;
static constexpr int window_height = 720;

float clamp_value(float value, float minimum, float maximum)
{
    if (value < minimum)
        return minimum;
    if (value > maximum)
        return maximum;
    return value;
}

Vector2 direction_to(Vector2 from, Vector2 to)
{
    Vector2 direction{to.x - from.x, to.y - from.y};
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length == 0.0f)
        return Vector2{0.0f, 0.0f};
    return Vector2{direction.x / length, direction.y / length};
}

class Player
{
public:
    void handle_input(float delta_time)
    {
        if (IsKeyDown(KEY_W))
            _position.y -= _speed * delta_time;
        if (IsKeyDown(KEY_S))
            _position.y += _speed * delta_time;
        if (IsKeyDown(KEY_A))
            _position.x -= _speed * delta_time;
        if (IsKeyDown(KEY_D))
            _position.x += _speed * delta_time;
    }
    void update()
    {
        _position.x = clamp_value(_position.x, 20.0f, window_width - 20.0f);
        _position.y = clamp_value(_position.y, 20.0f, window_height - 20.0f);
    }
    Vector2 position() const { return _position; }
    bool collides_with(Vector2 position, float radius) const { return CheckCollisionCircles(_position, _radius, position, radius); }
    void draw() const { DrawCircleV(_position, _radius, GREEN); }

private:
    Vector2 _position{window_width / 2.0f, window_height / 2.0f};
    float _speed = 260.0f;
    float _radius = 20.0f;
};

class Enemy
{
public:
    explicit Enemy(Vector2 position) : _position(position) {}
    void update(Vector2 target, float delta_time)
    {
        const Vector2 d = direction_to(_position, target);
        _position.x += d.x * _speed * delta_time;
        _position.y += d.y * _speed * delta_time;
    }
    bool collides_with(Vector2 position, float radius) const { return CheckCollisionCircles(_position, _radius, position, radius); }
    Vector2 position() const { return _position; }
    void draw() const { DrawCircleV(_position, _radius, RED); }

private:
    Vector2 _position;
    float _speed = 70.0f;
    float _radius = 16.0f;
};

class Bullet
{
public:
    explicit Bullet(float angle) : _angle(angle) {}
    void update(float delta_time) { _angle += _rotation_speed * delta_time; }
    Vector2 position(Vector2 player_position) const { return Vector2{player_position.x + std::cos(_angle) * _orbit_radius, player_position.y + std::sin(_angle) * _orbit_radius}; }
    bool collides_with(Vector2 player_position, const Enemy &enemy) const { return CheckCollisionCircles(position(player_position), _radius, enemy.position(), 16.0f); }
    void draw(Vector2 player_position) const { DrawCircleV(position(player_position), _radius, YELLOW); }

private:
    float _angle;
    float _radius = 7.0f;
    float _orbit_radius = 90.0f;
    float _rotation_speed = 2.5f;
};

Enemy create_enemy()
{
    const int edge = GetRandomValue(0, 3);
    if (edge == 0)
        return Enemy{Vector2{static_cast<float>(GetRandomValue(0, window_width)), -20.0f}};
    if (edge == 1)
        return Enemy{Vector2{static_cast<float>(GetRandomValue(0, window_width)), window_height + 20.0f}};
    if (edge == 2)
        return Enemy{Vector2{-20.0f, static_cast<float>(GetRandomValue(0, window_height))}};
    return Enemy{Vector2{window_width + 20.0f, static_cast<float>(GetRandomValue(0, window_height))}};
}

void reset_game(Player &player, std::vector<Enemy> &enemies, int &score, bool &game_over)
{
    player = Player{};
    enemies.clear();
    score = 0;
    game_over = false;
}

int main()
{
    InitWindow(window_width, window_height, "MiniSurvivors - 03 Combat");
    SetTargetFPS(60);

    Player player;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets{Bullet{0.0f}, Bullet{PI / 2.0f}, Bullet{PI}, Bullet{PI * 1.5f}};
    float spawn_timer = 0.0f;
    int score = 0;
    bool game_over = false;

    while (!WindowShouldClose())
    {
        const float delta_time = GetFrameTime();
        if (!game_over)
            player.handle_input(delta_time);
        if (game_over && IsKeyPressed(KEY_R))
            reset_game(player, enemies, score, game_over);

        if (!game_over)
        {
            player.update();
            spawn_timer += delta_time;
            if (spawn_timer >= 1.0f)
            {
                enemies.push_back(create_enemy());
                spawn_timer = 0.0f;
            }
            for (Bullet &bullet : bullets)
                bullet.update(delta_time);
            for (Enemy &enemy : enemies)
            {
                enemy.update(player.position(), delta_time);
                if (player.collides_with(enemy.position(), 16.0f))
                    game_over = true;
            }
            for (size_t enemy_index = 0; enemy_index < enemies.size();)
            {
                bool hit = false;
                for (const Bullet &bullet : bullets)
                    if (bullet.collides_with(player.position(), enemies[enemy_index]))
                    {
                        hit = true;
                        break;
                    }
                if (hit)
                {
                    enemies.erase(enemies.begin() + enemy_index);
                    ++score;
                }
                else
                    ++enemy_index;
            }
        }

        BeginDrawing();
        ClearBackground(Color{24, 28, 36, 255});
        player.draw();
        for (const Bullet &bullet : bullets)
            bullet.draw(player.position());
        for (const Enemy &enemy : enemies)
            enemy.draw();
        DrawText(TextFormat("Score: %d", score), 20, 20, 20, RAYWHITE);
        if (game_over)
        {
            DrawText("GAME OVER", 500, 300, 40, RAYWHITE);
            DrawText("Press R to restart", 500, 350, 20, RAYWHITE);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
