#include "application.h"

Application* Application::instance()
{
    static Application app;
    return &app;
}

Application::~Application()
{
    shutdown();
}

bool Application::init(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    if (_initialized)
    {
        return true;
    }

    InitWindow(kWindowWidth, kWindowHeight, "Raylib Minimal Template");
    SetTargetFPS(60);

    _initialized = true;
    return true;
}

int Application::run(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    if (!_initialized && !init(argc, argv))
    {
        return -1;
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{24, 28, 36, 255});

        DrawRectangle((kWindowWidth - 320) / 2, (kWindowHeight - 180) / 2, 320, 180,
                      Color{99, 179, 237, 255});
        DrawText("Raylib Template", 500, 475, 30, RAYWHITE);

        EndDrawing();
    }

    shutdown();
    return 0;
}

void Application::shutdown()
{
    if (_initialized)
    {
        CloseWindow();
        _initialized = false;
    }
}
