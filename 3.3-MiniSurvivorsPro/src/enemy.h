#pragma once

#include <memory>

#include <raylib.h>

#include "animation.h"
#include "enemy_ai.h"
#include "enemy_type.h"

class Enemy
{
public:
    Enemy(Vector2 position, EnemyType type, EnemyStats stats, std::unique_ptr<EnemyAI> ai);
    ~Enemy();
    Enemy(Enemy&& other) noexcept;
    Enemy& operator=(Enemy&& other) noexcept;
    Enemy(const Enemy& other) = delete;
    Enemy& operator=(const Enemy& other) = delete;

    void load();
    void unload();
    void update(Vector2 player_position, float delta_time);
    bool take_damage(int damage);
    Vector2 position() const;
    float radius() const;
    EnemyType type() const;
    int health() const;
    int max_health() const;
    int score_value() const;
    bool is_dead() const;
    bool collides_with(Vector2 position, float radius) const;
    void draw() const;
    void draw_health_bar() const;

private:
    Vector2 _position;
    EnemyType _type;
    EnemyStats _stats;
    int _health;
    float _hit_cooldown_timer = 0.0f;
    bool _facing_left = true;
    std::unique_ptr<EnemyAI> _ai;
    Texture2D _shadow{};
    Animation _animation;
};
