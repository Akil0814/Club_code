#include <vector>

#include <raylib.h>

#include "bullet.h"
#include "enemy.h"
#include "enemy_factory.h"
#include "game_config.h"
#include "player.h"

void draw_background(Texture2D background)
{
    const Rectangle source{0.0f, 0.0f, static_cast<float>(background.width), static_cast<float>(background.height)};
    const Rectangle destination{0.0f, 0.0f, static_cast<float>(window_width), static_cast<float>(window_height)};
    DrawTexturePro(background, source, destination, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

std::vector<Bullet> create_bullets()
{
    return std::vector<Bullet>{Bullet{0.0f}, Bullet{PI / 2.0f}, Bullet{PI}, Bullet{PI * 1.5f}};
}

void unload_enemies(std::vector<Enemy>& enemies)
{
    for (Enemy& enemy : enemies)
    {
        enemy.unload();
    }
    enemies.clear();
}

int main()
{
    InitWindow(window_width, window_height, "MiniSurvivors Pro");
    InitAudioDevice();
    SetTargetFPS(60);

    Texture2D background = LoadTexture("res/world/background.png");
    Sound hit_sound = LoadSound("res/audio/sfx_hit.wav");
    Music bgm = LoadMusicStream("res/audio/music_gameplay.mp3");
    PlayMusicStream(bgm);

    Player player;
    player.load();
    std::vector<Enemy> enemies;
    enemies.reserve(max_enemy_count);
    std::vector<Bullet> bullets = create_bullets();
    float spawn_timer = 0.0f;
    float elapsed_time = 0.0f;
    int score = 0;
    bool game_over = false;

    while (!WindowShouldClose())
    {
        const float delta_time = GetFrameTime();
        UpdateMusicStream(bgm);

        // input
        if (!game_over)
        {
            player.handle_input(delta_time);
        }
        if (game_over && IsKeyPressed(KEY_R))
        {
            player.unload();
            player = Player{};
            player.load();
            unload_enemies(enemies);
            bullets = create_bullets();
            spawn_timer = 0.0f;
            elapsed_time = 0.0f;
            score = 0;
            game_over = false;
        }

        // update
        if (!game_over)
        {
            player.update(delta_time);
            elapsed_time += delta_time;
            spawn_timer += delta_time;

            if (spawn_timer >= enemy_spawn_interval && enemies.size() < max_enemy_count)
            {
                enemies.push_back(EnemyFactory::create_random(elapsed_time));
                spawn_timer = 0.0f;
            }

            for (Bullet& bullet : bullets)
            {
                bullet.update(delta_time);
            }

            for (Enemy& enemy : enemies)
            {
                enemy.update(player.position(), delta_time);
                if (player.collides_with(enemy.position(), enemy.radius()) && player.take_damage(1))
                {
                    PlaySound(hit_sound);
                    if (player.is_dead()) game_over = true;
                }
            }

            for (std::size_t enemy_index = 0; enemy_index < enemies.size();)
            {
                Enemy& enemy = enemies[enemy_index];
                for (const Bullet& bullet : bullets)
                {
                    if (!bullet.collides_with(player.position(), enemy)) continue;
                    if (enemy.take_damage(bullet.damage())) PlaySound(hit_sound);
                    break;
                }

                if (enemy.is_dead())
                {
                    score += enemy.score_value();
                    enemy.unload();
                    enemies.erase(enemies.begin() + enemy_index);
                }
                else
                {
                    ++enemy_index;
                }
            }
        }

        // render
        BeginDrawing();
        draw_background(background);
        player.draw();
        for (const Enemy& enemy : enemies) enemy.draw();
        for (const Bullet& bullet : bullets) bullet.draw(player.position());
        DrawText(TextFormat("Score: %d", score), 20, 20, 20, RAYWHITE);
        player.draw_health_bar();

        if (game_over)
        {
            DrawRectangle(430, 285, 420, 130, Fade(BLACK, 0.7f));
            DrawText("GAME OVER", 510, 315, 40, RAYWHITE);
            DrawText("Press R to restart", 540, 365, 20, RAYWHITE);
        }
        EndDrawing();
    }

    player.unload();
    unload_enemies(enemies);
    UnloadTexture(background);
    UnloadSound(hit_sound);
    StopMusicStream(bgm);
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
