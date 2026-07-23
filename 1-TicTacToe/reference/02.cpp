#include <raylib.h>
#include <iostream>

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

    //...
}


int main()
{
    char board[3][3];
    char current_player = 'X';
    char winner = '-';

    InitWindow(window_width, window_height, "Tic Tac Toe");
    SetTargetFPS(60);

    reset_board(board);

    while(!WindowShouldClose())
    {
        //input
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            const int mouse_x = GetMouseX();
            const int mouse_y = GetMouseY();

            if (mouse_x >= 0 && mouse_x < window_width 
                &&mouse_y >= 0 && mouse_y < window_height)
            {
                const int column = mouse_x / 300;
                const int row = mouse_y / 300;

                if (board[row][column] == '-')
                {
                    board[row][column] = current_player;

                    if (check_win(board, current_player))
                        winner = current_player;

                    else if (is_board_full(board))
                        winner = 'D';

                    else
                        current_player = current_player == 'X' ? 'O' : 'X';
                    }

            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_board(board);

        DrawText(TextFormat("Turn: %c", current_player), 20, 20, 24, BLACK);

        if(winner != '-')
        {
            if (winner == 'D')
                DrawText("Draw!", 350, 370, 50, BLACK);
            else
                DrawText(TextFormat("%c Wins!", winner), 330, 370, 50, BLACK);
        }


        EndDrawing();
    }

    CloseWindow();

    return 0;
}