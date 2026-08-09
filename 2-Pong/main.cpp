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
    ball_position = Vector2{ window_width / 2.0f, window_height / 2.0f };

    ball_velocity = Vector2{ ball_speed * direction, ball_speed * 0.35f };
}

void move_paddles(Rectangle& left_paddle, Rectangle& right_paddle, float delta_time)
{
    if (IsKeyDown(KEY_W))
        left_paddle.y -= paddle_speed * delta_time;
    if (IsKeyDown(KEY_S)) 
        left_paddle.y += paddle_speed * delta_time;

    if (IsKeyDown(KEY_UP)) 
        right_paddle.y -= paddle_speed * delta_time;
    if (IsKeyDown(KEY_DOWN)) 
        right_paddle.y += paddle_speed * delta_time;
}

void keep_paddle_in_window(Rectangle& paddle)
{
    if (paddle.y < 0.0f) paddle.y = 0.0f;

    if (paddle.y > window_height - paddle.height)
        paddle.y = window_height - paddle.height;
}

void move_ball(Vector2& ball_position, const Vector2& ball_velocity, float delta_time)
{
    ball_position.x += ball_velocity.x * delta_time;
    ball_position.y += ball_velocity.y * delta_time;
}

void bounce_ball_on_walls(const Vector2& ball_position, Vector2& ball_velocity)
{
    if (ball_position.y - ball_radius <= 0.0f ||ball_position.y + ball_radius >= window_height)
    {
        ball_velocity.y *= -1.0f;
    }
}

void bounce_ball_on_paddles( Vector2& ball_position, Vector2& ball_velocity,
    const Rectangle& left_paddle, const Rectangle& right_paddle)
{
    if (CheckCollisionCircleRec(ball_position, ball_radius, left_paddle) && ball_velocity.x < 0.0f)
    {
        ball_position.x = left_paddle.x + left_paddle.width + ball_radius;
        ball_velocity.x *= -1.0f;
    }

    if (CheckCollisionCircleRec(ball_position, ball_radius, right_paddle) && ball_velocity.x > 0.0f)
    {
        ball_position.x = right_paddle.x - ball_radius;
        ball_velocity.x *= -1.0f;
    }
}

void update_score(Vector2& ball_position, Vector2& ball_velocity, 
    int& left_score, int& right_score)
{
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
}

void draw_game(const Rectangle& left_paddle,const Rectangle& right_paddle,
    const Vector2& ball_position,int left_score,int right_score)
{
    DrawRectangleRec(left_paddle, RAYWHITE);
    DrawRectangleRec(right_paddle, RAYWHITE);
    DrawCircleV(ball_position, ball_radius, RAYWHITE);
    DrawLine(window_width / 2, 0, window_width / 2, window_height, GRAY);
    DrawText(TextFormat("%d", left_score), window_width / 2 - 80, 30, 40, RAYWHITE);
    DrawText(TextFormat("%d", right_score), window_width / 2 + 55, 30, 40, RAYWHITE);
}

int main()
{
    InitWindow(window_width, window_height, "Simple Pong");
    SetTargetFPS(60);

    Rectangle left_paddle{ 40.0f,
        window_height / 2.0f - paddle_height / 2.0f,
        paddle_width,paddle_height };

    Rectangle right_paddle{ window_width - 40.0f - paddle_width,
        window_height / 2.0f - paddle_height / 2.0f,
        paddle_width, paddle_height};

    Vector2 ball_position{};
    Vector2 ball_velocity{};
    reset_ball(ball_position, ball_velocity, 1.0f);

    int left_score = 0;
    int right_score = 0;

    while (!WindowShouldClose())
    {
        const float delta_time = GetFrameTime();

        // input
        move_paddles(left_paddle, right_paddle, delta_time);

        // update
        keep_paddle_in_window(left_paddle);
        keep_paddle_in_window(right_paddle);
        move_ball(ball_position, ball_velocity, delta_time);
        bounce_ball_on_walls(ball_position, ball_velocity);
        bounce_ball_on_paddles(ball_position, ball_velocity, left_paddle, right_paddle);
        update_score(ball_position, ball_velocity, left_score, right_score);

        // render
        BeginDrawing();
        ClearBackground(BLACK);

        draw_game(left_paddle, right_paddle, ball_position, left_score, right_score);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
