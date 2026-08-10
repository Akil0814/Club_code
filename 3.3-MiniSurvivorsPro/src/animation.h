#pragma once

#include <array>

#include <raylib.h>

#include "game_config.h"

class Animation
{
public:
    void load(const char* path_pattern);
    void update(float delta_time);
    void draw(Vector2 center) const;
    void unload();

private:
    std::array<Texture2D, animation_frame_count> _frames{};
    int _current_frame = 0;
    float _timer = 0.0f;
};

