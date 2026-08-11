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

void Animation::draw(Vector2 center, Color tint, float scale) const
{
    const Texture2D texture = _frames[_current_frame];
    const Rectangle source{0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};
    const Rectangle destination{
        center.x,
        center.y,
        texture.width * scale,
        texture.height * scale,
    };
    const Vector2 origin{destination.width / 2.0f, destination.height / 2.0f};
    DrawTexturePro(texture, source, destination, origin, 0.0f, tint);
}

void Animation::unload()
{
    for (Texture2D frame : _frames)
    {
        UnloadTexture(frame);
    }
}

