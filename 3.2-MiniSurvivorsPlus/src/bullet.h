#pragma once

#include <raylib.h>

class Enemy;

class Bullet
{
public:
    explicit Bullet(float angle);
    void update(float delta_time);
    Vector2 position(Vector2 player_position) const;
    bool collides_with(Vector2 player_position, const Enemy& enemy) const;
    void draw(Vector2 player_position) const;

private:
    float _angle;
    float _radius = 7.0f;
    float _orbit_radius = 90.0f;
    float _rotation_speed = 2.5f;
};
