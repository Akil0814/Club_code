#include <raylib.h>

static constexpr int window_width = 1280;
static constexpr int window_height = 720;

float clamp_value(float value, float minimum, float maximum)
{
    if (value < minimum) return minimum;
    if (value > maximum) return maximum;
    return value;
}

class Player
{
public:
    void handle_input(float delta_time)
    {
        if (IsKeyDown(KEY_W)) _position.y -= _speed * delta_time;
        if (IsKeyDown(KEY_S)) _position.y += _speed * delta_time;
        if (IsKeyDown(KEY_A)) _position.x -= _speed * delta_time;
        if (IsKeyDown(KEY_D)) _position.x += _speed * delta_time;
    }

    void update()
    {
        _position.x = clamp_value(_position.x, _radius, window_width - _radius);
        _position.y = clamp_value(_position.y, _radius, window_height - _radius);
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

int main()
{
    InitWindow(window_width, window_height, "MiniSurvivors - 01 Player");
    SetTargetFPS(60);

    Player player;
    while (!WindowShouldClose())
    {
        const float delta_time = GetFrameTime();

        // input
        player.handle_input(delta_time);

        // update
        player.update();

        // render
        BeginDrawing();
        ClearBackground(Color{24, 28, 36, 255});
        player.draw();
        DrawText("W A S D to move", 20, 20, 20, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
