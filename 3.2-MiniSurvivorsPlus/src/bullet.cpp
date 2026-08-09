#include "bullet.h"

#include <cmath>

#include "enemy.h"

Bullet::Bullet(float angle) : _angle(angle)
{
}

void Bullet::update(float delta_time)
{
    _angle += _rotation_speed * delta_time;
}

Vector2 Bullet::position(Vector2 player_position) const
{
    return Vector2{
        player_position.x + std::cos(_angle) * _orbit_radius,
        player_position.y + std::sin(_angle) * _orbit_radius,
    };
}

bool Bullet::collides_with(Vector2 player_position, const Enemy& enemy) const
{
    return CheckCollisionCircles(position(player_position), _radius, enemy.position(), 24.0f);
}

void Bullet::draw(Vector2 player_position) const
{
    DrawCircleV(position(player_position), _radius, YELLOW);
}
