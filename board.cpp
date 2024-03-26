#include <algorithm>

#include "board.h"

// Set up board
void initBoard(GameBoard *board, int init_size)
{
    board->size = init_size;
    board->Blocks = new Block *[board->size];
    
    for (int i = 0; i < board->size; i++)
    {
        board->Blocks[i] = new Block[board->size];
    }
}

void fillBoard(GameBoard *board)
{
    int pool_size = board->size * board->size;
    int *pool = new int[pool_size];
    
    genRandoms(pool, pool_size);
    shuffleRandoms(pool, pool_size);

    for (int i = 0; i < board->size; i++)
    {
        for (int j = 0; j < board->size; j++)
        {
            char block_value = pool[pool_size - 1] + 'A';
            initBLock(board->Blocks[i][j], j, i, block_value);
            pool_size--;
        }
    }

    delete[] pool;
}


// Blocks checking
bool check_I_Match(GameBoard board, Block first, Block second)
{
    bool check_vertical = true;
    bool check_horizontal = true;

    // Check vertically
    if (first.x != second.x)
        check_vertical = false;
    else
    {
        // Check if there's any other block in the way.
        int start = std::min(first.y, second.y); 
        int end = std::max(first.y, second.y);

        for (int row = start + 1; row < end; row++)
            if (board.Blocks[row][first.x].mode != EMPTY)
            {
                check_vertical = false;
                break;
            }
    }

    // Check horizontally
    if (first.y != second.y)
        check_horizontal = false;
    else
    {
        // Check if there's any other block in the way.
        int start = std::min(first.x, second.x); 
        int end = std::max(first.x, second.x);

        for (int col = start + 1; col < end; col++)
            if (board.Blocks[first.y][col].mode != EMPTY)
            {
                check_horizontal = false;
                break;
            }
    }

    return check_vertical || check_horizontal;
}

bool check_U_Top(GameBoard board, Block first, Block second)
{
    // Check top
    int top_y = std::min(first.y, second.y) - 1;
    int flag_negative = 0;
    if (top_y < 0)
    {
        flag_negative = 1;
        top_y++;
    }

    Block tmp1 = board.Blocks[top_y][first.x];
    Block tmp2 = board.Blocks[top_y][second.x]; 

    // Scan empty straight path between input blocks and temporary blocks
    if (!check_I_Match(board, first, tmp1) || !check_I_Match(board, second, tmp2))
        return false;

    if (!flag_negative)
    {
        // Check path between temporary blocks
        // If path not clear, decrease top_y and continue checking until reaching boundary
        while (top_y >= 0 && !check_I_Match(board, tmp1, tmp2))
            tmp1.y = tmp2.y = top_y--;

        if (top_y < 0)
            return true;

        if (!check_I_Match(board, tmp1, tmp2) || (tmp1.mode != EMPTY || tmp2.mode != EMPTY))
            return false;
    }

    return true;
}

bool check_U_Bottom(GameBoard board, Block first, Block second)
{
    // Check bottom
    int bottom_y = std::max(first.y, second.y) + 1;
    int flag_out_of_rows = 0;
    if (bottom_y >= board.size)
    {
        flag_out_of_rows = 1;
        bottom_y--;
    }

    Block tmp1 = board.Blocks[bottom_y][first.x];
    Block tmp2 = board.Blocks[bottom_y][second.x]; 

    // Scan empty straight path between input blocks and temporary blocks
    if (!check_I_Match(board, first, tmp1) || !check_I_Match(board, second, tmp2))
        return false;

    if (!flag_out_of_rows)
    {
        // Check path between temporary blocks
        // If path not clear, increase bottom_y and continue checking until reaching boundary
        while (bottom_y < board.size && !check_I_Match(board, tmp1, tmp2))
            tmp1.y = tmp2.y = bottom_y++;

        if (bottom_y >= board.size)
            return true;

        if (!check_I_Match(board, tmp1, tmp2) || (tmp1.mode != EMPTY || tmp2.mode != EMPTY))
            return false;
    }

    return true;
}

bool check_U_Left(GameBoard board, Block first, Block second)
{
    // Check left
    int left_x = std::min(first.x, second.x) - 1;
    int flag_negative = 0;
    if (left_x < 0)
    {
        flag_negative = 1;
        left_x++;
    }

    Block tmp1 = board.Blocks[first.y][left_x];
    Block tmp2 = board.Blocks[second.y][left_x]; 

    // Scan empty straight path between input blocks and temporary blocks
    if (!check_I_Match(board, first, tmp1) || !check_I_Match(board, second, tmp2))
        return false;

    if (!flag_negative)
    {
        // Check path between temporary blocks
        // If path not clear, decrease left_x and continue checking until reaching boundary
        while (left_x >= 0 && !check_I_Match(board, tmp1, tmp2))
            tmp1.x = tmp2.x = left_x--;

        if (left_x < 0)
            return true;

        if (!check_I_Match(board, tmp1, tmp2) || (tmp1.mode != EMPTY || tmp2.mode != EMPTY))
            return false;
    }

    return true;
}

bool check_U_Right(GameBoard board, Block first, Block second)
{
    // Check bottom
    int right_x = std::max(first.y, second.y) + 1;
    int flag_out_of_cols = 0;
    if (right_x >= board.size)
    {
        flag_out_of_cols = 1;
        right_x--;
    }

    Block tmp1 = board.Blocks[first.y][right_x];
    Block tmp2 = board.Blocks[second.y][right_x]; 

    // Scan empty path between input blocks and temporary blocks
    if (!check_I_Match(board, first, tmp1) || !check_I_Match(board, second, tmp2))
        return false;

    if (!flag_out_of_cols)
    {
        // Check path between temporary blocks
        // If path not clear, increase bottom_y and continue checking until reaching boundary
        while (right_x < board.size && !check_I_Match(board, tmp1, tmp2))
            tmp1.x = tmp2.x = right_x++;

        if (right_x >= board.size)
            return true;

        if (!check_I_Match(board, tmp1, tmp2) || (tmp1.mode != EMPTY || tmp2.mode != EMPTY))
            return false;
    }

    return true;
}

bool check_U_Match(GameBoard board, Block first, Block second)
{
    bool check_top = check_U_Top(board, first, second);
    bool check_bottom = check_U_Bottom(board, first, second);
    bool check_left = check_U_Left(board, first, second);
    bool check_right = check_U_Right(board, first, second);

    return check_top || check_bottom || check_left || check_right;
}


// Clean up board
void cleanBoard(GameBoard *board)
{
    for (int i = 0; i < board->size; i++)
        delete[] board->Blocks[i];

    delete[] board->Blocks;
}
