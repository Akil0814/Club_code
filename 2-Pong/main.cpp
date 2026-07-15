#include <raylib.h>

static constexpr int window_width = 960;
static constexpr int window_height = 540;

static constexpr float paddle_width = 20.0f;
static constexpr float paddle_height = 100.0f;
static constexpr float paddle_speed = 420.0f;
static constexpr float ball_radius = 10.0f;
static constexpr float ball_speed = 360.0f;

void reset_ball(Vector2& ball_position, Vector2& ball_velocity, float direction)
{
    ball_position = Vector2{
        window_width / 2.0f,
        window_height / 2.0f,
    };

    ball_velocity = Vector2{
        ball_speed * direction,
        ball_speed * 0.35f,
    };
}

int main()
{
    InitWindow(window_width, window_height, "Simple Pong");
    SetTargetFPS(60);

    Rectangle left_paddle{
        40.0f,
        window_height / 2.0f - paddle_height / 2.0f,
        paddle_width,
        paddle_height,
    };

    Rectangle right_paddle{
        window_width - 40.0f - paddle_width,
        window_height / 2.0f - paddle_height / 2.0f,
        paddle_width,
        paddle_height,
    };

    Vector2 ball_position{};
    Vector2 ball_velocity{};
    reset_ball(ball_position, ball_velocity, 1.0f);

    int left_score = 0;
    int right_score = 0;

    while (!WindowShouldClose())
    {
        const float delta_time = GetFrameTime();

        // input
        if (IsKeyDown(KEY_W))
        {
            left_paddle.y -= paddle_speed * delta_time;
        }
        if (IsKeyDown(KEY_S))
        {
            left_paddle.y += paddle_speed * delta_time;
        }
        if (IsKeyDown(KEY_UP))
        {
            right_paddle.y -= paddle_speed * delta_time;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            right_paddle.y += paddle_speed * delta_time;
        }

        // update
        if (left_paddle.y < 0.0f)
        {
            left_paddle.y = 0.0f;
        }
        if (left_paddle.y > window_height - left_paddle.height)
        {
            left_paddle.y = window_height - left_paddle.height;
        }
        if (right_paddle.y < 0.0f)
        {
            right_paddle.y = 0.0f;
        }
        if (right_paddle.y > window_height - right_paddle.height)
        {
            right_paddle.y = window_height - right_paddle.height;
        }

        ball_position.x += ball_velocity.x * delta_time;
        ball_position.y += ball_velocity.y * delta_time;

        if (ball_position.y - ball_radius <= 0.0f ||
            ball_position.y + ball_radius >= window_height)
        {
            ball_velocity.y *= -1.0f;
        }

        if (CheckCollisionCircleRec(ball_position, ball_radius, left_paddle) &&
            ball_velocity.x < 0.0f)
        {
            ball_position.x = left_paddle.x + left_paddle.width + ball_radius;
            ball_velocity.x *= -1.0f;
        }

        if (CheckCollisionCircleRec(ball_position, ball_radius, right_paddle) &&
            ball_velocity.x > 0.0f)
        {
            ball_position.x = right_paddle.x - ball_radius;
            ball_velocity.x *= -1.0f;
        }

        if (ball_position.x < -ball_radius)
        {
            ++right_score;
            reset_ball(ball_position, ball_velocity, 1.0f);
        }
        else if (ball_position.x > window_width + ball_radius)
        {
            ++left_score;
            reset_ball(ball_position, ball_velocity, -1.0f);
        }

        // render
        BeginDrawing();
        ClearBackground(Color{24, 28, 36, 255});

        DrawRectangle(
            static_cast<int>(left_paddle.x),
            static_cast<int>(left_paddle.y),
            static_cast<int>(left_paddle.width),
            static_cast<int>(left_paddle.height),
            RAYWHITE);
        DrawRectangle(
            static_cast<int>(right_paddle.x),
            static_cast<int>(right_paddle.y),
            static_cast<int>(right_paddle.width),
            static_cast<int>(right_paddle.height),
            RAYWHITE);
        DrawCircleV(ball_position, ball_radius, RAYWHITE);
        DrawLine(window_width / 2, 0, window_width / 2, window_height, GRAY);

        DrawText(TextFormat("%d", left_score), window_width / 2 - 80, 30, 40, RAYWHITE);
        DrawText(TextFormat("%d", right_score), window_width / 2 + 55, 30, 40, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
