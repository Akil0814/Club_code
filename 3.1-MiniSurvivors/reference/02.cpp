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
        _position.x = clamp_value(_position.x, _radius, window_width - _radius);
        _position.y = clamp_value(_position.y, _radius, window_height - _radius);
    }

    Vector2 position() const
    {
        return _position;
    }
    void draw() const
    {
        DrawCircleV(_position, _radius, GREEN);
    }

private:
    Vector2 _position{window_width / 2.0f, window_height / 2.0f};
    float _speed = 260.0f;
    float _radius = 20.0f;
};

class Enemy
{
public:
    explicit Enemy(Vector2 position) : _position(position)
    {
    }

    void update(Vector2 target, float delta_time)
    {
        const Vector2 direction = direction_to(_position, target);
        _position.x += direction.x * _speed * delta_time;
        _position.y += direction.y * _speed * delta_time;
    }

    void draw() const
    {
        DrawCircleV(_position, _radius, RED);
    }

private:
    Vector2 _position;
    float _speed = 70.0f;
    float _radius = 16.0f;
};

Enemy create_enemy()
{
    const int edge = GetRandomValue(0, 3);
    if (edge == 0)
        return Enemy{Vector2{static_cast<float>(GetRandomValue(0, window_width)), -20.0f}};
    if (edge == 1)
        return Enemy{
            Vector2{static_cast<float>(GetRandomValue(0, window_width)), window_height + 20.0f}};
    if (edge == 2)
        return Enemy{Vector2{-20.0f, static_cast<float>(GetRandomValue(0, window_height))}};
    return Enemy{
        Vector2{window_width + 20.0f, static_cast<float>(GetRandomValue(0, window_height))}};
}

int main()
{
    InitWindow(window_width, window_height, "MiniSurvivors - 02 Enemies");
    SetTargetFPS(60);

    Player player;
    std::vector<Enemy> enemies;
    float spawn_timer = 0.0f;

    while (!WindowShouldClose())
    {
        const float delta_time = GetFrameTime();

        // input
        player.handle_input(delta_time);

        // update
        player.update();
        spawn_timer += delta_time;
        if (spawn_timer >= 1.0f)
        {
            enemies.push_back(create_enemy());
            spawn_timer = 0.0f;
        }
        for (Enemy& enemy : enemies)
            enemy.update(player.position(), delta_time);

        // render
        BeginDrawing();
        ClearBackground(Color{24, 28, 36, 255});
        player.draw();
        for (const Enemy& enemy : enemies)
            enemy.draw();
        DrawText("W A S D to move", 20, 20, 20, RAYWHITE);
        DrawText(TextFormat("Enemies: %d", static_cast<int>(enemies.size())), 20, 48, 20, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
