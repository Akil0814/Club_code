#include <raylib.h>

static constexpr int window_width = 900;
static constexpr int window_height = 900;
static constexpr int board_size = 3;
static constexpr int cell_size = window_width / board_size;
static constexpr int piece_padding = 54;

enum class GameResult
{
    Playing,
    XWins,
    OWins,
    Draw
};

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
            if (board[row][column] == ' ')
            {
                return false;
            }
        }
    }

    return true;
}

GameResult check_game_result(const char board[board_size][board_size])
{
    for (int row = 0; row < board_size; ++row)
    {
        if (board[row][0] != ' ' &&
            board[row][0] == board[row][1] &&
            board[row][1] == board[row][2])
        {
            return board[row][0] == 'X' ? GameResult::XWins : GameResult::OWins;
        }
    }

    for (int column = 0; column < board_size; ++column)
    {
        if (board[0][column] != ' ' &&
            board[0][column] == board[1][column] &&
            board[1][column] == board[2][column])
        {
            return board[0][column] == 'X' ? GameResult::XWins : GameResult::OWins;
        }
    }

    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
    {
        return board[0][0] == 'X' ? GameResult::XWins : GameResult::OWins;
    }

    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
    {
        return board[0][2] == 'X' ? GameResult::XWins : GameResult::OWins;
    }

    if (is_board_full(board))
    {
        return GameResult::Draw;
    }

    return GameResult::Playing;
}

bool try_get_clicked_cell(Vector2 mouse_position, int& row, int& column)
{
    if (mouse_position.x < 0.0f ||
        mouse_position.x >= static_cast<float>(window_width) ||
        mouse_position.y < 0.0f ||
        mouse_position.y >= static_cast<float>(window_height))
    {
        return false;
    }

    column = static_cast<int>(mouse_position.x) / cell_size;
    row = static_cast<int>(mouse_position.y) / cell_size;
    return true;
}

void draw_grid()
{
    for (int index = 1; index < board_size; ++index)
    {
        const int position = index * cell_size;

        DrawLine(position, 0, position, window_height, BLACK);
        DrawLine(0, position, window_width, position, BLACK);
    }
}

void draw_piece_x(int row, int column)
{
    const int left = column * cell_size + piece_padding;
    const int top = row * cell_size + piece_padding;
    const int right = (column + 1) * cell_size - piece_padding;
    const int bottom = (row + 1) * cell_size - piece_padding;

    DrawLine(left, top, right, bottom, BLACK);
    DrawLine(right, top, left, bottom, BLACK);
}

void draw_piece_o(int row, int column)
{
    const int center_x = column * cell_size + cell_size / 2;
    const int center_y = row * cell_size + cell_size / 2;
    const float radius = static_cast<float>(cell_size / 2 - piece_padding);

    DrawCircleLines(center_x, center_y, radius, BLACK);
}

void draw_board(const char board[board_size][board_size])
{
    draw_grid();

    for (int row = 0; row < board_size; ++row)
    {
        for (int column = 0; column < board_size; ++column)
        {
            if (board[row][column] == 'X')
            {
                draw_piece_x(row, column);
            }
            else if (board[row][column] == 'O')
            {
                draw_piece_o(row, column);
            }
        }
    }
}

void draw_turn_text(char current_player)
{
    const char* turn_text = current_player == 'X' ? "Turn: X" : "Turn: O";
    DrawText(turn_text, 12, 12, 24, BLACK);
}

void draw_popup(GameResult game_result)
{
    DrawRectangle(250, 330, 400, 160, RAYWHITE);
    DrawRectangleLines(250, 330, 400, 160, BLACK);

    if (game_result == GameResult::XWins)
    {
        DrawText("X Wins!", 330, 370, 50, BLACK);
    }
    else if (game_result == GameResult::OWins)
    {
        DrawText("O Wins!", 330, 370, 50, BLACK);
    }
    else
    {
        DrawText("Draw!", 350, 370, 50, BLACK);
    }

    DrawText("Click to restart", 305, 430, 26, BLACK);
}

int main()
{
    char board[board_size][board_size];
    char current_player = 'X';
    GameResult game_result = GameResult::Playing;

    reset_board(board);

    InitWindow(window_width, window_height, "Tic Tac Toe Easy");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (game_result == GameResult::Playing)
            {
                const Vector2 mouse_position = GetMousePosition();
                int row = 0;
                int column = 0;

                if (try_get_clicked_cell(mouse_position, row, column) && board[row][column] == ' ')
                {
                    board[row][column] = current_player;
                    game_result = check_game_result(board);

                    if (game_result == GameResult::Playing)
                    {
                        current_player = current_player == 'X' ? 'O' : 'X';
                    }
                }
            }
            else
            {
                reset_board(board);
                current_player = 'X';
                game_result = GameResult::Playing;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_board(board);
        draw_turn_text(current_player);

        if (game_result != GameResult::Playing)
        {
            draw_popup(game_result);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
