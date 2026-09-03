#include "player.h"

#include "math_utils.h"

void Player::load()
{
    _left_animation.load("res/player_left_%d.png");
    _right_animation.load("res/player_right_%d.png");
    _shadow = LoadTexture("res/shadow_player.png");
}

void Player::unload()
{
    _left_animation.unload();
    _right_animation.unload();
    UnloadTexture(_shadow);
}

void Player::handle_input(float delta_time)
{
    const Vector2 old_position = _position;

    if (IsKeyDown(KEY_W))
        _position.y -= _speed * delta_time;
    if (IsKeyDown(KEY_S))
        _position.y += _speed * delta_time;
    if (IsKeyDown(KEY_A))
        _position.x -= _speed * delta_time;
    if (IsKeyDown(KEY_D))
        _position.x += _speed * delta_time;

    if (_position.x < old_position.x)
        _facing_left = true;
    if (_position.x > old_position.x)
        _facing_left = false;
}

void Player::update(float delta_time)
{
    _position.x = clamp_value(_position.x, 32.0f, window_width - 32.0f);
    _position.y = clamp_value(_position.y, 32.0f, window_height - 32.0f);
    _left_animation.update(delta_time);
    _right_animation.update(delta_time);
}

Vector2 Player::position() const
{
    return _position;
}

bool Player::collides_with(Vector2 position, float radius) const
{
    return CheckCollisionCircles(_position, _radius, position, radius);
}

void Player::draw() const
{
    DrawTexture(_shadow, static_cast<int>(_position.x - _shadow.width / 2.0f),
                static_cast<int>(_position.y + 20.0f), WHITE);
    if (_facing_left)
        _left_animation.draw(_position);
    else
        _right_animation.draw(_position);
}
