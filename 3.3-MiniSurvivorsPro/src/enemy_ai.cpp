#include "enemy_ai.h"

#include "math_utils.h"

EnemyAI::~EnemyAI() = default;

Vector2 ChaseAI::movement_direction(Vector2 enemy_position, Vector2 player_position) const
{
    return direction_to(enemy_position, player_position);
}

KeepDistanceAI::KeepDistanceAI(float preferred_distance) : _preferred_distance(preferred_distance)
{
}

Vector2 KeepDistanceAI::movement_direction(Vector2 enemy_position, Vector2 player_position) const
{
    constexpr float distance_tolerance = 25.0f;
    const float distance = distance_between(enemy_position, player_position);
    const Vector2 direction = direction_to(enemy_position, player_position);

    if (distance > _preferred_distance + distance_tolerance) return direction;
    if (distance < _preferred_distance - distance_tolerance) return Vector2{-direction.x, -direction.y};
    return Vector2{0.0f, 0.0f};
}

