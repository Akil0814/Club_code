#include <vector>

#include <raylib.h>

#include "bullet.h"
#include "enemy.h"
#include "enemy_factory.h"
#include "game_config.h"
#include "player.h"

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
    InitWindow(window_width, window_height, "MiniSurvivors Plus");
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
                enemies.push_back(EnemyFactory::create_random());
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
