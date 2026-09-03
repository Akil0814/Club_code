#pragma once

class Scene
{
public:
    virtual ~Scene();
    virtual void on_enter() = 0;
    virtual void on_exit() = 0;
    virtual void update(float delta_time) = 0;
    virtual void draw() const = 0;
};
