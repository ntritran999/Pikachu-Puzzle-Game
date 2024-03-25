#pragma once

#include <new>

#include "block.h"
#include "block_logic.h"

struct GameBoard
{
    int size;       // Create a size x size grid.
    Block **Blocks; 
};

// Setup board
void initBoard(GameBoard *board, int init_size);
void fillBoard(GameBoard *board);
void drawBoard(GameBoard *board);

// Block Matching algorithms
bool check_I_Match(GameBoard board, Block first, Block second);
void check_L_Match(Block first, Block second);
bool check_U_Match(GameBoard board, Block first, Block second);
void check_Z_Match(Block first, Block second);


// Clean up board
void cleanBoard(GameBoard *board);