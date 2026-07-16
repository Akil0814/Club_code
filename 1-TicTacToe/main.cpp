#include <raylib.h>

static constexpr int window_width = 900;
static constexpr int window_height = 900;
static constexpr int board_size = 3;
static constexpr int cell_size = 300;

void reset_board(char board[board_size][board_size])
{
    for (int row = 0; row < board_size; ++row)
    {
        for (int column = 0; column < board_size; ++column)
        {
            board[row][column] = ' ';
        }
    }
}

bool is_board_full(const char board[board_size][board_size])
{
    for (int row = 0; row < board_size; ++row)
    {
        for (int column = 0; column < board_size; ++column)
        {
            if (board[row][column] == ' ') return false;
        }
    }

    return true;
}

bool check_win(const char board[board_size][board_size], char player)
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

void draw_board(const char board[board_size][board_size])
{
    // Draw the grid.
    for (int index = 1; index < board_size; ++index)
    {
        const int position = index * cell_size;
        DrawLine(position, 0, position, window_height, BLACK);
        DrawLine(0, position, window_width, position, BLACK);
    }

    // Draw every piece while visiting its cell.
    for (int row = 0; row < board_size; ++row)
    {
        for (int column = 0; column < board_size; ++column)
        {
            const int left = column * cell_size;
            const int top = row * cell_size;
            const int right = left + cell_size;
            const int bottom = top + cell_size;

            if (board[row][column] == 'X')
            {
                DrawLine(left + 50, top + 50, right - 50, bottom - 50, BLACK);
                DrawLine(right - 50, top + 50, left + 50, bottom - 50, BLACK);
            }
            else if (board[row][column] == 'O')
            {
                DrawCircleLines(left + cell_size / 2, top + cell_size / 2,
                    cell_size / 2.0f - 50.0f, BLACK);
            }
        }
    }
}

int main()
{
    char board[board_size][board_size];
    char current_player = 'X';
    char winner = ' ';

    reset_board(board);

    InitWindow(window_width, window_height, "Tic Tac Toe");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (winner != ' ')
            {
                reset_board(board);
                current_player = 'X';
                winner = ' ';
            }
            else
            {
                const int mouse_x = GetMouseX();
                const int mouse_y = GetMouseY();

                if (mouse_x >= 0 && mouse_x < window_width &&
                    mouse_y >= 0 && mouse_y < window_height)
                {
                    const int column = mouse_x / cell_size;
                    const int row = mouse_y / cell_size;

                    if (board[row][column] == ' ')
                    {
                        board[row][column] = current_player;
                        if (check_win(board, current_player))
                        {
                            winner = current_player;
                        }
                        else if (is_board_full(board))
                        {
                            winner = 'D';
                        }
                        else
                        {
                            current_player = current_player == 'X' ? 'O' : 'X';
                        }
                    }
                }
            }
        }

        // render
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_board(board);

        if (winner == ' ')
        {
            DrawText(TextFormat("Turn: %c", current_player), 20, 20, 24, BLACK);
        }
        else
        {
            DrawRectangle(250, 330, 400, 160, RAYWHITE);
            DrawRectangleLines(250, 330, 400, 160, BLACK);

            if (winner == 'D') DrawText("Draw!", 350, 370, 50, BLACK);
            else DrawText(TextFormat("%c Wins!", winner), 330, 370, 50, BLACK);

            DrawText("Click to restart", 305, 430, 26, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
