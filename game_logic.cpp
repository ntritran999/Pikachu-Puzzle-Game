#include "game_logic.h"

bool checkEmptyBoard(GameBoard board)
{
    for (int i = 0; i < board.size; i++)
        for (int j = 0; j < board.size; j++)
            if (board.Blocks[i][j].mode != EMPTY)
                return false;
    return true;
}

bool checkRemainPairs(GameBoard board)
{
    Block first, second;
    for (int i = 0; i < board.size; i++)
    {
        for (int j = 0; i < board.size; j++)
        {
            first = board.Blocks[i][j];
            for (int r = i; r < board.size; r++)
                for (int c = j; c < board.size; c++)
                {
                    second = board.Blocks[r][c];
                    bool check_I = check_I_Match(board, first, second);
                    bool check_L = check_L_Match(board, first, second);
                    bool check_Z = check_Z_Match(board, first, second);
                    bool check_U = check_U_Match(board, first, second);
                    
                    if (!check_I || !check_L || !check_Z || !check_U)
                        return false;
                }
        }
    }
    return true;
}
