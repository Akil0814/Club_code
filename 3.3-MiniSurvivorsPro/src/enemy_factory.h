#pragma once

#include <memory>

#include <raylib.h>

#include "enemy.h"

class EnemyFactory
{
public:
    static Enemy create(EnemyType type);
    static Enemy create_random(float elapsed_time);

private:
    static Vector2 random_spawn_position();
    static EnemyType random_type(float elapsed_time);
    static EnemyStats stats_for(EnemyType type);
    static std::unique_ptr<EnemyAI> create_ai(EnemyType type);
};
