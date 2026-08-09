#pragma once

#include <raylib.h>

#include "animation.h"

class Enemy
{
public:
    explicit Enemy(Vector2 position);
    void unload();
    void update(Vector2 target, float delta_time);
    Vector2 position() const;
    bool collides_with(Vector2 position, float radius) const;
    void draw() const;

private:
    Vector2 _position;
    float _speed = 70.0f;
    float _radius = 24.0f;
    bool _facing_left = false;
    Texture2D _shadow{};
    Animation _left_animation;
    Animation _right_animation;
};
