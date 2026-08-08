#include <raylib.h>

const int window_width = 900;
const int window_height = 900;

void reset_board(char board[3][3])
{
    for (int row = 0; row < 3; ++row)
    {
        for (int column = 0; column < 3; ++column)
        {
            board[row][column] = '-';
        }
    }
}

bool is_board_full(const char board[3][3])
{
    for (int row = 0; row < 3; ++row)
    {
        for (int column = 0; column < 3; ++column)
        {
            if (board[row][column] == '-')
                return false;
        }
    }

    return true;
}

bool check_win(const char board[3][3], char player)
{
    if (board[0][0] == player && board[0][1] == player && board[0][2] == player) return true;
    if (board[1][0] == player && board[1][1] == player && board[1][2] == player) return true;
    if (board[2][0] == player && board[2][1] == player && board[2][2] == player) return true;

    if (board[0][0] == player && board[1][0] == player && board[2][0] == player) return true;
    if (board[0][1] == player && board[1][1] == player && board[2][1] == player) return true;
    if (board[0][2] == player && board[1][2] == player && board[2][2] == player) return true;

    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return true;

    return false;
}

void draw_board(const char board[3][3])
{
    for (int index = 1; index < 3; ++index)
    {
        const int position = index * 300;
        DrawLine(position, 0, position, window_width, BLACK);
        DrawLine(0, position, window_height, position, BLACK);
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
                DrawLine(left, top, right, bottom, BLACK);
                DrawLine(right, top, left, bottom, BLACK);
            }
            else if (board[row][column] == 'O')
            {
                DrawCircleLines(left + 300 / 2, top + 300 / 2, 300 / 2.0f, BLACK);
            }
        }
    }
}

int main()
{
    char board[3][3];
    char current_player = 'X';
    char winner = '-';
    reset_board(board);

    InitWindow(window_width, window_height, "Tic Tac Toe");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // input
        //...

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
