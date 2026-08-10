#pragma once

#include <raylib.h>

class EnemyAI
{
public:
    virtual ~EnemyAI();
    virtual Vector2 movement_direction(Vector2 enemy_position, Vector2 player_position) const = 0;
};

class ChaseAI final : public EnemyAI
{
public:
    Vector2 movement_direction(Vector2 enemy_position, Vector2 player_position) const override;
};

class KeepDistanceAI final : public EnemyAI
{
public:
    explicit KeepDistanceAI(float preferred_distance);
    Vector2 movement_direction(Vector2 enemy_position, Vector2 player_position) const override;

private:
    float _preferred_distance;
};

