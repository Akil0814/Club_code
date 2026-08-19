#pragma once

#include <memory>

#include <raylib.h>

#include "animation.h"
#include "enemy_ai.h"
#include "enemy_attack_strategy.h"
#include "enemy_type.h"

class Enemy
{
public:
    Enemy(Vector2 position, EnemyType type, EnemyStats stats, std::unique_ptr<EnemyAI> ai,
          std::unique_ptr<EnemyAttackStrategy> attack_strategy);
    ~Enemy();
    Enemy(Enemy&& other) noexcept;
    Enemy& operator=(Enemy&& other) noexcept;
    Enemy(const Enemy& other) = delete;
    Enemy& operator=(const Enemy& other) = delete;

    void load();
    void unload();
    void update(Vector2 player_position, float delta_time);
    void take_damage(int damage);
    Vector2 position() const;
    EnemyType type() const;
    int health() const;
    int max_health() const;
    int score_value() const;
    int contact_damage() const;
    bool is_dead() const;
    bool collides_with(Vector2 position, float radius) const;
    std::optional<EnemyProjectileSpawn> update_attack(Vector2 player_position, float delta_time);
    void draw() const;
    void draw_health_bar() const;

private:
    Vector2 _position;
    EnemyType _type;
    EnemyStats _stats;
    int _health;
    bool _facing_left = false;
    std::unique_ptr<EnemyAI> _ai;
    std::unique_ptr<EnemyAttackStrategy> _attack_strategy;
    Texture2D _shadow{};
    Animation _left_animation;
    Animation _right_animation;
};
