#pragma once

#include <memory>
#include <unordered_map>

#include "scene.h"
#include "scene_type.h"

class SceneManager
{
public:
    void register_scene(SceneType type, std::unique_ptr<Scene> scene);
    void switch_to(SceneType type);
    void update(float delta_time);
    void draw() const;
    void request_exit();
    bool should_exit() const;
    SceneType current_scene() const;

private:
    std::unordered_map<SceneType, std::unique_ptr<Scene>> _scenes;
    Scene* _current_scene = nullptr;
    SceneType _current_type = SceneType::MainMenu;
    bool _should_exit = false;
};
