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
    if (first.coord_x != second.coord_x)
        check_vertical = false;
    else
    {
        // Check if there's any other block in the way.
        int start, end;
        if (first.coord_y < second.coord_y)
            start = first.coord_y;
        else
            end = second.coord_y;

        for (int row = start + 1; row < end; row++)
            if (board.Blocks[row][first.coord_x].mode != EMPTY)
            {
                check_vertical = false;
                break;
            }

        // Check for matching character
        if (first.value != second.value)
            check_vertical = false;
    }

    // Check horizontally
    if (first.coord_y != second.coord_y)
        check_horizontal = false;
    else
    {
        // Check if there's any other block in the way.
        int start, end;
        if (first.coord_x < second.coord_x)
            start = first.coord_x;
        else
            end = second.coord_x;

        for (int col = start + 1; col < end; col++)
            if (board.Blocks[first.coord_y][col].mode != EMPTY)
            {
                check_horizontal = false;
                break;
            }

        // Check for matching character
        if (first.value != second.value)
            check_horizontal = false;
    }

    return check_vertical || check_horizontal;
}

bool check_U_Match(GameBoard board, Block first, Block second)
{

}


// Clean up board
void cleanBoard(GameBoard *board)
{
    for (int i = 0; i < board->size; i++)
        delete[] board->Blocks[i];

    delete[] board->Blocks;
}
