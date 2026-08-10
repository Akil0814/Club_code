#pragma once

#include <raylib.h>

enum class EnemyType
{
    Normal,
    Fast,
    Tank,
    Cautious
};

struct EnemyStats
{
    float speed;
    float radius;
    int max_health;
    int score_value;
    int contact_damage;
    Color tint;
};
