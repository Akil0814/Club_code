#pragma once

#include <raylib.h>

#include "enemy_attack_strategy.h"

class Player;

class EnemyBullet
{
public:
    explicit EnemyBullet(const EnemyProjectileSpawn& spawn);
    void update(float delta_time);
    Vector2 position() const;
    float radius() const;
    int damage() const;
    bool is_outside_window() const;
    bool collides_with(const Player& player) const;
    void draw() const;

private:
    Vector2 _position;
    Vector2 _velocity;
    float _radius;
    int _damage;
};
