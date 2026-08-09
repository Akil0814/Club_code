#include "enemy.h"

#include "math_utils.h"

Enemy::Enemy(Vector2 position) : _position(position)
{
    _left_animation.load("res/enemy_left_%d.png");
    _right_animation.load("res/enemy_right_%d.png");
    _shadow = LoadTexture("res/shadow_enemy.png");
}

void Enemy::unload()
{
    _left_animation.unload();
    _right_animation.unload();
    UnloadTexture(_shadow);
}

void Enemy::update(Vector2 target, float delta_time)
{
    const Vector2 direction = direction_to(_position, target);
    _position.x += direction.x * _speed * delta_time;
    _position.y += direction.y * _speed * delta_time;
    _facing_left = direction.x < 0.0f;
    _left_animation.update(delta_time);
    _right_animation.update(delta_time);
}

Vector2 Enemy::position() const
{
    return _position;
}

bool Enemy::collides_with(Vector2 position, float radius) const
{
    return CheckCollisionCircles(_position, _radius, position, radius);
}

void Enemy::draw() const
{
    DrawTexture(_shadow, static_cast<int>(_position.x - _shadow.width / 2.0f), static_cast<int>(_position.y + 20.0f), WHITE);
    if (_facing_left) _left_animation.draw(_position);
    else _right_animation.draw(_position);
}
