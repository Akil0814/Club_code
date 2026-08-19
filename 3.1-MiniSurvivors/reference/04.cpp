#include <array>
#include <cmath>
#include <vector>

#include <raylib.h>

static constexpr int window_width = 1280;
static constexpr int window_height = 720;
static constexpr int animation_frame_count = 6;

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

class Animation
{
public:
    void load(const char* path_pattern)
    {
        for (int frame = 0; frame < animation_frame_count; ++frame)
        {
            _frames[frame] = LoadTexture(TextFormat(path_pattern, frame));
        }
    }

    void update(float delta_time)
    {
        _timer += delta_time;
        if (_timer >= 0.10f)
        {
            _timer = 0.0f;
            _current_frame = (_current_frame + 1) % animation_frame_count;
        }
    }

    void draw(Vector2 center) const
    {
        const Texture2D texture = _frames[_current_frame];
        DrawTexture(texture, static_cast<int>(center.x - texture.width / 2.0f),
                    static_cast<int>(center.y - texture.height / 2.0f), WHITE);
    }

    void unload()
    {
        for (Texture2D frame : _frames)
            UnloadTexture(frame);
    }

private:
    std::array<Texture2D, animation_frame_count> _frames{};
    int _current_frame = 0;
    float _timer = 0.0f;
};

class Player
{
public:
    void load()
    {
        _left_animation.load("res/player_left_%d.png");
        _right_animation.load("res/player_right_%d.png");
        _shadow = LoadTexture("res/shadow_player.png");
    }

    void unload()
    {
        _left_animation.unload();
        _right_animation.unload();
        UnloadTexture(_shadow);
    }

    void handle_input(float delta_time)
    {
        const Vector2 old_position = _position;

        if (IsKeyDown(KEY_W))
            _position.y -= _speed * delta_time;
        if (IsKeyDown(KEY_S))
            _position.y += _speed * delta_time;
        if (IsKeyDown(KEY_A))
            _position.x -= _speed * delta_time;
        if (IsKeyDown(KEY_D))
            _position.x += _speed * delta_time;

        if (_position.x < old_position.x)
            _facing_left = true;
        if (_position.x > old_position.x)
            _facing_left = false;
    }

    void update(float delta_time)
    {
        _position.x = clamp_value(_position.x, 32.0f, window_width - 32.0f);
        _position.y = clamp_value(_position.y, 32.0f, window_height - 32.0f);

        _left_animation.update(delta_time);
        _right_animation.update(delta_time);
    }

    Vector2 position() const
    {
        return _position;
    }
    bool collides_with(Vector2 position, float radius) const
    {
        return CheckCollisionCircles(_position, _radius, position, radius);
    }

    void draw() const
    {
        DrawTexture(_shadow, static_cast<int>(_position.x - _shadow.width / 2.0f),
                    static_cast<int>(_position.y + 20.0f), WHITE);
        if (_facing_left)
            _left_animation.draw(_position);
        else
            _right_animation.draw(_position);
    }

private:
    Vector2 _position{window_width / 2.0f, window_height / 2.0f};
    float _speed = 260.0f;
    float _radius = 24.0f;
    bool _facing_left = false;
    Texture2D _shadow{};
    Animation _left_animation;
    Animation _right_animation;
};

class Enemy
{
public:
    explicit Enemy(Vector2 position) : _position(position)
    {
        _left_animation.load("res/enemy_left_%d.png");
        _right_animation.load("res/enemy_right_%d.png");
        _shadow = LoadTexture("res/shadow_enemy.png");
    }

    void unload()
    {
        _left_animation.unload();
        _right_animation.unload();
        UnloadTexture(_shadow);
    }

    void update(Vector2 target, float delta_time)
    {
        const Vector2 direction = direction_to(_position, target);
        _position.x += direction.x * _speed * delta_time;
        _position.y += direction.y * _speed * delta_time;
        _facing_left = direction.x < 0.0f;
        _left_animation.update(delta_time);
        _right_animation.update(delta_time);
    }

    Vector2 position() const
    {
        return _position;
    }
    bool collides_with(Vector2 position, float radius) const
    {
        return CheckCollisionCircles(_position, _radius, position, radius);
    }

    void draw() const
    {
        DrawTexture(_shadow, static_cast<int>(_position.x - _shadow.width / 2.0f),
                    static_cast<int>(_position.y + 20.0f), WHITE);
        if (_facing_left)
            _left_animation.draw(_position);
        else
            _right_animation.draw(_position);
    }

private:
    Vector2 _position;
    float _speed = 70.0f;
    float _radius = 24.0f;
    bool _facing_left = false;
    Texture2D _shadow{};
    Animation _left_animation;
    Animation _right_animation;
};

class Bullet
{
public:
    explicit Bullet(float angle) : _angle(angle)
    {
    }
    void update(float delta_time)
    {
        _angle += _rotation_speed * delta_time;
    }
    Vector2 position(Vector2 player_position) const
    {
        return Vector2{player_position.x + std::cos(_angle) * _orbit_radius,
                       player_position.y + std::sin(_angle) * _orbit_radius};
    }
    bool collides_with(Vector2 player_position, const Enemy& enemy) const
    {
        return CheckCollisionCircles(position(player_position), _radius, enemy.position(), 24.0f);
    }
    void draw(Vector2 player_position) const
    {
        DrawCircleV(position(player_position), _radius, YELLOW);
    }

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
        return Enemy{Vector2{static_cast<float>(GetRandomValue(0, window_width)), -32.0f}};
    if (edge == 1)
        return Enemy{
            Vector2{static_cast<float>(GetRandomValue(0, window_width)), window_height + 32.0f}};
    if (edge == 2)
        return Enemy{Vector2{-32.0f, static_cast<float>(GetRandomValue(0, window_height))}};

    return Enemy{
        Vector2{window_width + 32.0f, static_cast<float>(GetRandomValue(0, window_height))}};
}

void draw_background(Texture2D background)
{
    const Rectangle source{0.0f, 0.0f, static_cast<float>(background.width),
                           static_cast<float>(background.height)};
    const Rectangle destination{0.0f, 0.0f, static_cast<float>(window_width),
                                static_cast<float>(window_height)};
    DrawTexturePro(background, source, destination, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

int main()
{
    InitWindow(window_width, window_height, "MiniSurvivors - 04 Assets");
    InitAudioDevice();
    SetTargetFPS(60);

    Texture2D background = LoadTexture("res/background.png");
    Sound hit_sound = LoadSound("res/hit.wav");
    Music bgm = LoadMusicStream("res/bgm.mp3");
    PlayMusicStream(bgm);

    Player player;
    player.load();
    std::vector<Enemy> enemies;
    enemies.reserve(32);
    std::vector<Bullet> bullets{Bullet{0.0f}, Bullet{PI / 2.0f}, Bullet{PI}, Bullet{PI * 1.5f}};
    float spawn_timer = 0.0f;
    int score = 0;
    bool game_over = false;

    while (!WindowShouldClose())
    {
        const float delta_time = GetFrameTime();
        UpdateMusicStream(bgm);

        // input
        if (!game_over)
            player.handle_input(delta_time);
        if (game_over && IsKeyPressed(KEY_R))
        {
            player.unload();
            player = Player{};
            player.load();
            for (Enemy& enemy : enemies)
                enemy.unload();
            enemies.clear();
            score = 0;
            game_over = false;
        }

        // update
        if (!game_over)
        {
            player.update(delta_time);
            spawn_timer += delta_time;
            if (spawn_timer >= 1.0f && enemies.size() < 32)
            {
                enemies.push_back(create_enemy());
                spawn_timer = 0.0f;
            }
            for (Bullet& bullet : bullets)
                bullet.update(delta_time);
            for (Enemy& enemy : enemies)
            {
                enemy.update(player.position(), delta_time);
                if (player.collides_with(enemy.position(), 24.0f))
                    game_over = true;
            }
            for (size_t enemy_index = 0; enemy_index < enemies.size();)
            {
                bool hit = false;
                for (const Bullet& bullet : bullets)
                {
                    if (bullet.collides_with(player.position(), enemies[enemy_index]))
                    {
                        hit = true;
                        break;
                    }
                }
                if (hit)
                {
                    PlaySound(hit_sound);
                    enemies[enemy_index].unload();
                    enemies.erase(enemies.begin() + enemy_index);
                    ++score;
                }
                else
                    ++enemy_index;
            }
        }

        // render
        BeginDrawing();
        draw_background(background);
        player.draw();
        for (const Enemy& enemy : enemies)
            enemy.draw();
        for (const Bullet& bullet : bullets)
            bullet.draw(player.position());
        DrawText(TextFormat("Score: %d", score), 20, 20, 20, RAYWHITE);
        if (game_over)
        {
            DrawRectangle(430, 285, 420, 130, Color{0, 0, 0, 180});
            DrawText("GAME OVER", 510, 315, 40, RAYWHITE);
            DrawText("Press R to restart", 540, 365, 20, RAYWHITE);
        }
        EndDrawing();
    }

    player.unload();
    for (Enemy& enemy : enemies)
        enemy.unload();
    UnloadTexture(background);
    UnloadSound(hit_sound);
    StopMusicStream(bgm);
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
