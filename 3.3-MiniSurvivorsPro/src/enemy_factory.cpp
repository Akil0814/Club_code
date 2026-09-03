#include "enemy_factory.h"

#include "game_config.h"

Enemy EnemyFactory::create(EnemyType type)
{
    Enemy enemy{random_spawn_position(), type, stats_for(type), create_ai(type)};
    enemy.load();
    return enemy;
}

Enemy EnemyFactory::create_random(float elapsed_time)
{
    return create(random_type(elapsed_time));
}

Vector2 EnemyFactory::random_spawn_position()
{
    const int edge = GetRandomValue(0, 3);
    if (edge == 0)
        return Vector2{static_cast<float>(GetRandomValue(0, window_width)), -32.0f};
    if (edge == 1)
        return Vector2{static_cast<float>(GetRandomValue(0, window_width)), window_height + 32.0f};
    if (edge == 2)
        return Vector2{-32.0f, static_cast<float>(GetRandomValue(0, window_height))};
    return Vector2{window_width + 32.0f, static_cast<float>(GetRandomValue(0, window_height))};
}

EnemyType EnemyFactory::random_type(float elapsed_time)
{
    const int roll = GetRandomValue(1, 100);

    if (elapsed_time < 20.0f)
    {
        if (roll <= 65)
            return EnemyType::Normal;
        if (roll <= 85)
            return EnemyType::Fast;
        if (roll <= 95)
            return EnemyType::Tank;
        return EnemyType::Cautious;
    }

    if (elapsed_time < 45.0f)
    {
        if (roll <= 40)
            return EnemyType::Normal;
        if (roll <= 65)
            return EnemyType::Fast;
        if (roll <= 85)
            return EnemyType::Tank;
        return EnemyType::Cautious;
    }

    if (roll <= 25)
        return EnemyType::Normal;
    if (roll <= 55)
        return EnemyType::Fast;
    if (roll <= 80)
        return EnemyType::Tank;
    return EnemyType::Cautious;
}

EnemyStats EnemyFactory::stats_for(EnemyType type)
{
    switch (type)
    {
    case EnemyType::Normal:
        return EnemyStats{70.0f, 24.0f, 1, 1, WHITE};
    case EnemyType::Fast:
        return EnemyStats{115.0f, 20.0f, 1, 2, ORANGE};
    case EnemyType::Tank:
        return EnemyStats{45.0f, 30.0f, 4, 4, RED};
    case EnemyType::Cautious:
        return EnemyStats{60.0f, 22.0f, 2, 3, SKYBLUE};
    }

    return EnemyStats{70.0f, 24.0f, 1, 1, WHITE};
}

std::unique_ptr<EnemyAI> EnemyFactory::create_ai(EnemyType type)
{
    if (type == EnemyType::Cautious)
    {
        return std::make_unique<KeepDistanceAI>(cautious_preferred_distance);
    }
    return std::make_unique<ChaseAI>();
}
