#pragma once

#include "button.h"
#include "game_session.h"
#include "scene.h"

class SceneManager;

class MainMenuScene final : public Scene
{
public:
    MainMenuScene(SceneManager& scene_manager, GameSession& session);
    void on_enter() override;
    void on_exit() override;
    void update(float delta_time) override;
    void draw() const override;

private:
    SceneManager& _scene_manager;
    GameSession& _session;
    Button _start_button;
    Button _exit_button;
};
