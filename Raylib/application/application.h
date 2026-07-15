#pragma once

#include <raylib.h>

class Application
{
public:
    static Application* instance();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    bool init(int argc, char** argv);
    int run(int argc, char** argv);
    void shutdown();

private:
    Application() = default;
    ~Application();

    static constexpr int kWindowWidth = 1280;
    static constexpr int kWindowHeight = 720;

    bool _initialized = false;
};
