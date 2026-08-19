#include "animation.h"

void Animation::load(const char* path_pattern, int frame_count)
{
    _frames.reserve(frame_count);
    for (int frame = 0; frame < frame_count; ++frame)
    {
        _frames.push_back(LoadTexture(TextFormat(path_pattern, frame)));
    }
    _current_frame = 0;
    _timer = 0.0f;
}

void Animation::update(float delta_time)
{
    if (_frames.empty())
        return;

    _timer += delta_time;
    if (_timer >= 0.10f)
    {
        _timer = 0.0f;
        _current_frame = (_current_frame + 1) % static_cast<int>(_frames.size());
    }
}

void Animation::draw(Vector2 center, Color tint, float scale, bool flip_horizontal) const
{
    if (_frames.empty())
        return;

    const Texture2D texture = _frames[_current_frame];
    const float source_width =
        flip_horizontal ? -static_cast<float>(texture.width) : static_cast<float>(texture.width);
    const Rectangle source{0.0f, 0.0f, source_width, static_cast<float>(texture.height)};
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
    _frames.clear();
    _current_frame = 0;
    _timer = 0.0f;
}
