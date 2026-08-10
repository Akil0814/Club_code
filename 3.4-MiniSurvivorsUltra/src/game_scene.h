#pragma once

#include <vector>

#include "bullet.h"
#include "enemy.h"
#include "enemy_bullet.h"
#include "game_session.h"
#include "player.h"
#include "scene.h"

class SceneManager;

class GameScene final : public Scene
{
public:
    GameScene(SceneManager& scene_manager, GameSession& session);
    void on_enter() override;
    void on_exit() override;
    void update(float delta_time) override;
    void draw() const override;

private:
    void reset_game();
    void update_enemy_spawning(float delta_time);
    void update_collisions();
    void update_enemy_attacks(float delta_time);
    void remove_finished_entities();
    void finish_game();
    void draw_background() const;

    SceneManager& _scene_manager;
    GameSession& _session;
    Player _player;
    std::vector<Enemy> _enemies;
    std::vector<Bullet> _bullets;
    std::vector<EnemyBullet> _enemy_bullets;
    float _elapsed_time = 0.0f;
    float _spawn_timer = 0.0f;
};

