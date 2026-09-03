#pragma once

#include <optional>

#include <raylib.h>

struct EnemyProjectileSpawn
{
    Vector2 position;
    Vector2 velocity;
    float radius;
    int damage;
};

class EnemyAttackStrategy
{
public:
    virtual ~EnemyAttackStrategy();
    virtual std::optional<EnemyProjectileSpawn> update(Vector2 enemy_position,
                                                       Vector2 player_position,
                                                       float delta_time) = 0;
};

class NoAttackStrategy final : public EnemyAttackStrategy
{
public:
    std::optional<EnemyProjectileSpawn> update(Vector2 enemy_position, Vector2 player_position,
                                               float delta_time) override;
};

class RangedAttackStrategy final : public EnemyAttackStrategy
{
public:
    RangedAttackStrategy(float attack_interval, float bullet_speed, float bullet_radius,
                         int damage);
    std::optional<EnemyProjectileSpawn> update(Vector2 enemy_position, Vector2 player_position,
                                               float delta_time) override;

private:
    float _attack_interval;
    float _attack_timer = 0.0f;
    float _bullet_speed;
    float _bullet_radius;
    int _damage;
};
