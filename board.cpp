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
            initBLock(board->Blocks[i][j], i, j, block_value);
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
        int start, end;
        if (first.y < second.y)
        {
            start = first.y;
            end = second.y;
        }
        else
        {
            start = second.y;
            end = first.y;
        }

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
        int start, end;
        if (first.x < second.x)
        {
            start = first.x;
            end = second.x;
        }
        else
        {
            start = second.x;
            end = first.x;
        }

        for (int col = start + 1; col < end; col++)
            if (board.Blocks[first.y][col].mode != EMPTY)
            {
                check_horizontal = false;
                break;
            }
    }

    return check_vertical || check_horizontal;
}

bool check_U_Match(GameBoard board, Block first, Block second)
{
    bool check_horizontal = true, check_vertical = true;

    // Check horizontal
    if (first.y != second.y)
        check_horizontal = false;
    else
    {
        // Check top and bottom
        // First to check top, check the two blocks right above first and second block
        // Then perform I-match checking for the blocks bewteen those two tmp blocks
        // Do the same thing again for checking bottom
        for (int i = -1; i <= 1; i += 2)
        {
            Block tmp1 = board.Blocks[first.y + i][first.x];
            Block tmp2 = board.Blocks[second.y + i][second.x];
            if(!check_I_Match(board, tmp1, tmp2) || tmp1.mode != EMPTY || tmp2.mode != EMPTY)
                check_horizontal = false;
        }
    }
    
    // Check vertical
    if (first.x != second.x)
        check_vertical = false;
    else
    {
        // Check top and bottom
        // First to check top, check the two blocks on the left of first and second block
        // Then perform I-match checking for the blocks bewteen those two tmp blocks
        // Do the same thing again for checking right
        for (int i = -1; i <= 1; i += 2)
        {
            Block tmp1 = board.Blocks[first.y][first.x + i];
            Block tmp2 = board.Blocks[second.y][second.x + i];
            if(!check_I_Match(board, tmp1, tmp2) || tmp1.mode != EMPTY || tmp2.mode != EMPTY)
                check_vertical = false;
        }
    }

    return check_horizontal || check_vertical;
}


// Clean up board
void cleanBoard(GameBoard *board)
{
    for (int i = 0; i < board->size; i++)
        delete[] board->Blocks[i];

    delete[] board->Blocks;
}
