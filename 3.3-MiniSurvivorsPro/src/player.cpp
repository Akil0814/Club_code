#include "player.h"

#include <algorithm>

#include "math_utils.h"

void Player::load()
{
    _animation.load("res/players/player_01/move_%02d.png", 6);
    _shadow = LoadTexture("res/players/shadow.png");
}

void Player::unload()
{
    _animation.unload();
    UnloadTexture(_shadow);
}

void Player::handle_input(float delta_time)
{
    const Vector2 old_position = _position;
    if (IsKeyDown(KEY_W)) _position.y -= _speed * delta_time;
    if (IsKeyDown(KEY_S)) _position.y += _speed * delta_time;
    if (IsKeyDown(KEY_A)) _position.x -= _speed * delta_time;
    if (IsKeyDown(KEY_D)) _position.x += _speed * delta_time;
    if (_position.x < old_position.x) _facing_left = true;
    if (_position.x > old_position.x) _facing_left = false;
}

void Player::update(float delta_time)
{
    _position.x = clamp_value(_position.x, 32.0f, window_width - 32.0f);
    _position.y = clamp_value(_position.y, 32.0f, window_height - 32.0f);
    _invulnerability_timer = std::max(0.0f, _invulnerability_timer - delta_time);
    _animation.update(delta_time);
}

bool Player::take_damage(int damage)
{
    if (damage <= 0 || is_dead() || is_invulnerable()) return false;

    _health = std::max(0, _health - damage);
    _invulnerability_timer = player_invulnerability_duration;
    return true;
}

Vector2 Player::position() const
{
    return _position;
}

int Player::health() const
{
    return _health;
}

int Player::max_health() const
{
    return player_max_health;
}

bool Player::is_dead() const
{
    return _health <= 0;
}

bool Player::is_invulnerable() const
{
    return _invulnerability_timer > 0.0f;
}

bool Player::collides_with(Vector2 position, float radius) const
{
    return CheckCollisionCircles(_position, _radius, position, radius);
}

void Player::draw() const
{
    const bool faded = is_invulnerable() && static_cast<int>(_invulnerability_timer * 10.0f) % 2 == 0;
    const Color tint = faded ? Fade(WHITE, 0.35f) : WHITE;

    DrawTexture(
        _shadow,
        static_cast<int>(_position.x - _shadow.width / 2.0f),
        static_cast<int>(_position.y + 20.0f),
        tint
    );
    _animation.draw(_position, tint, 1.0f, !_facing_left);
}

void Player::draw_health_bar() const
{
    constexpr int bar_x = 20;
    constexpr int bar_y = 50;
    constexpr int bar_width = 200;
    constexpr int bar_height = 18;
    const float health_ratio = static_cast<float>(_health) / player_max_health;

    DrawRectangle(bar_x, bar_y, bar_width, bar_height, DARKGRAY);
    DrawRectangle(bar_x, bar_y, static_cast<int>(bar_width * health_ratio), bar_height, GREEN);
    DrawRectangleLines(bar_x, bar_y, bar_width, bar_height, RAYWHITE);
    DrawText(TextFormat("HP: %d / %d", _health, player_max_health), bar_x + 6, bar_y + 2, 14, RAYWHITE);
}
