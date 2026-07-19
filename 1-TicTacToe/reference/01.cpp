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

void draw_board()
{
    for (int index = 1; index < 3; ++index)
    {
        const int position = index * 300;
        DrawLine(position, 0, position, 900, BLACK);
        DrawLine(0, position, 900, position, BLACK);
    }
}

int main()
{
    char board[3][3];
    reset_board(board);

    InitWindow(900, 900, "Tic Tac Toe - 01 Grid");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_board();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
