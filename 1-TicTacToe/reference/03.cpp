#include <raylib.h>

const int window_width = 900;
const int window_height = 900;

void reset_board(char board[3][3])
{
    for (int row = 0; row < 3; ++row)
    {
        for (int column = 0; column < 3; ++column)
        {
            board[row][column] = ' ';
        }
    }
}

void draw_board(const char board[3][3])
{
    for (int index = 1; index < 3; ++index)
    {
        const int position = index * 300;
        DrawLine(position, 0, position, 900, BLACK);
        DrawLine(0, position, 900, position, BLACK);
    }

    for (int row = 0; row < 3; ++row)
    {
        for (int column = 0; column < 3; ++column)
        {
            const int left = column * 300;
            const int top = row * 300;
            const int right = left + 300;
            const int bottom = top + 300;

            if (board[row][column] == 'X')
            {
                DrawLine(left + 40, top + 40, right - 40, bottom - 40, BLACK);
                DrawLine(right - 40, top + 40, left + 40, bottom - 40, BLACK);
            }
            else if (board[row][column] == 'O')
            {
                DrawCircleLines(left + 300 / 2, top + 300 / 2,
                    300 / 2.0f - 40.0f, BLACK);
            }
        }
    }
}

int main()
{
    char board[3][3];
    char current_player = 'X';
    reset_board(board);

    InitWindow(900, 900, "Tic Tac Toe - 03 Input");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            const int mouse_x = GetMouseX();
            const int mouse_y = GetMouseY();

            if (mouse_x >= 0 && mouse_x < 900 &&
                mouse_y >= 0 && mouse_y < 900)
            {
                const int column = mouse_x / 300;
                const int row = mouse_y / 300;

                if (board[row][column] == ' ')
                {
                    board[row][column] = current_player;
                    current_player = current_player == 'X' ? 'O' : 'X';
                }
            }
        }

        // render
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_board(board);
        DrawText(TextFormat("Turn: %c", current_player), 20, 20, 24, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
