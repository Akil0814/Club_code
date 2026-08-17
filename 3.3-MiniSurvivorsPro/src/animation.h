#pragma once

#include <vector>

#include <raylib.h>

class Animation
{
public:
    void load(const char* path_pattern, int frame_count);
    void update(float delta_time);
    void draw(
        Vector2 center,
        Color tint = WHITE,
        float scale = 1.0f,
        bool flip_horizontal = false
    ) const;
    void unload();

private:
    std::vector<Texture2D> _frames;
    int _current_frame = 0;
    float _timer = 0.0f;
};
