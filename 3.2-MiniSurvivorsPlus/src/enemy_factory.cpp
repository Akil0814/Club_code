#include "enemy_factory.h"

#include <raylib.h>

#include "game_config.h"

Enemy EnemyFactory::create_random()
{
    const int edge = GetRandomValue(0, 3);
    if (edge == 0)
        return Enemy{Vector2{static_cast<float>(GetRandomValue(0, window_width)), -32.0f}};
    if (edge == 1)
        return Enemy{
            Vector2{static_cast<float>(GetRandomValue(0, window_width)), window_height + 32.0f}};
    if (edge == 2)
        return Enemy{Vector2{-32.0f, static_cast<float>(GetRandomValue(0, window_height))}};
    return Enemy{
        Vector2{window_width + 32.0f, static_cast<float>(GetRandomValue(0, window_height))}};
}
