#include "animation.h"

void Animation::load(const char* path_pattern)
{
    for (int frame = 0; frame < animation_frame_count; ++frame)
    {
        _frames[frame] = LoadTexture(TextFormat(path_pattern, frame));
    }
}

void Animation::update(float delta_time)
{
    _timer += delta_time;
    if (_timer >= 0.10f)
    {
        _timer = 0.0f;
        _current_frame = (_current_frame + 1) % animation_frame_count;
    }
}

void Animation::draw(Vector2 center) const
{
    const Texture2D texture = _frames[_current_frame];
    DrawTexture(texture,
        static_cast<int>(center.x - texture.width / 2.0f),
        static_cast<int>(center.y - texture.height / 2.0f),
        WHITE);
}

void Animation::unload()
{
    for (Texture2D frame : _frames) UnloadTexture(frame);
}
