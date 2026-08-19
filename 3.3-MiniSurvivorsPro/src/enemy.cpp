#include "enemy.h"

#include <algorithm>
#include <utility>

#include "game_config.h"

Enemy::Enemy(Vector2 position, EnemyType type, EnemyStats stats, std::unique_ptr<EnemyAI> ai)
    : _position(position), _type(type), _stats(stats), _health(stats.max_health), _ai(std::move(ai))
{
}

Enemy::~Enemy() = default;
Enemy::Enemy(Enemy&& other) noexcept = default;
Enemy& Enemy::operator=(Enemy&& other) noexcept = default;

void Enemy::load()
{
    _animation.load("res/enemies/boar/move_%02d.png", 6);
    _shadow = LoadTexture("res/enemies/shadow.png");
}

void Enemy::unload()
{
    _animation.unload();
    UnloadTexture(_shadow);
}

void Enemy::update(Vector2 player_position, float delta_time)
{
    _hit_cooldown_timer = std::max(0.0f, _hit_cooldown_timer - delta_time);
    const Vector2 direction = _ai->movement_direction(_position, player_position);
    _position.x += direction.x * _stats.speed * delta_time;
    _position.y += direction.y * _stats.speed * delta_time;
    if (direction.x < 0.0f)
        _facing_left = true;
    if (direction.x > 0.0f)
        _facing_left = false;
    _animation.update(delta_time);
}

bool Enemy::take_damage(int damage)
{
    if (damage <= 0 || is_dead() || _hit_cooldown_timer > 0.0f)
        return false;

    _health = std::max(0, _health - damage);
    _hit_cooldown_timer = enemy_hit_cooldown;
    return true;
}

Vector2 Enemy::position() const
{
    return _position;
}

float Enemy::radius() const
{
    return _stats.radius;
}

EnemyType Enemy::type() const
{
    return _type;
}

int Enemy::health() const
{
    return _health;
}

int Enemy::max_health() const
{
    return _stats.max_health;
}

int Enemy::score_value() const
{
    return _stats.score_value;
}

bool Enemy::is_dead() const
{
    return _health <= 0;
}

bool Enemy::collides_with(Vector2 position, float radius) const
{
    return CheckCollisionCircles(_position, _stats.radius, position, radius);
}

void Enemy::draw() const
{
    const float scale = _stats.radius / 24.0f;
    const Rectangle source{0.0f, 0.0f, static_cast<float>(_shadow.width),
                           static_cast<float>(_shadow.height)};
    const Rectangle destination{
        _position.x,
        _position.y + 20.0f,
        _shadow.width * scale,
        _shadow.height * scale,
    };
    const Vector2 origin{destination.width / 2.0f, 0.0f};
    DrawTexturePro(_shadow, source, destination, origin, 0.0f, _stats.tint);

    _animation.draw(_position, _stats.tint, scale, !_facing_left);
    draw_health_bar();
}

void Enemy::draw_health_bar() const
{
    if (_stats.max_health <= 1)
        return;

    const float bar_width = _stats.radius * 2.0f;
    constexpr float bar_height = 6.0f;
    const float bar_x = _position.x - bar_width / 2.0f;
    const float bar_y = _position.y - _stats.radius - 16.0f;
    const float health_ratio = static_cast<float>(_health) / _stats.max_health;

    DrawRectangleRec(Rectangle{bar_x, bar_y, bar_width, bar_height}, DARKGRAY);
    DrawRectangleRec(Rectangle{bar_x, bar_y, bar_width * health_ratio, bar_height}, GREEN);
    DrawRectangleLinesEx(Rectangle{bar_x, bar_y, bar_width, bar_height}, 1.0f, RAYWHITE);
}
