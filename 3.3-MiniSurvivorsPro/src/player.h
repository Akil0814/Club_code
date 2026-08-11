#pragma once

#include <raylib.h>

#include "animation.h"
#include "game_config.h"

class Player
{
public:
    void load();
    void unload();
    void handle_input(float delta_time);
    void update(float delta_time);
    bool take_damage(int damage);
    Vector2 position() const;
    int health() const;
    int max_health() const;
    bool is_dead() const;
    bool is_invulnerable() const;
    bool collides_with(Vector2 position, float radius) const;
    void draw() const;
    void draw_health_bar() const;

private:
    Vector2 _position{window_width / 2.0f, window_height / 2.0f};
    float _speed = 260.0f;
    float _radius = 24.0f;
    int _health = player_max_health;
    float _invulnerability_timer = 0.0f;
    bool _facing_left = false;
    Texture2D _shadow{};
    Animation _left_animation;
    Animation _right_animation;
};
