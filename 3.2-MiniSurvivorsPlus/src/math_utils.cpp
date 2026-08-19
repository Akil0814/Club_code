#include "math_utils.h"

#include <cmath>

float clamp_value(float value, float minimum, float maximum)
{
    if (value < minimum)
        return minimum;
    if (value > maximum)
        return maximum;
    return value;
}

Vector2 direction_to(Vector2 from, Vector2 to)
{
    Vector2 direction{to.x - from.x, to.y - from.y};
    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length == 0.0f)
        return Vector2{0.0f, 0.0f};
    return Vector2{direction.x / length, direction.y / length};
}
