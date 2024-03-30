#pragma once

#include <new>
#include <algorithm>
#include <iostream>

#include "block.h"
#include "block_logic.h"
#include "window_handler.h"

struct GameBoard
{
    int size;       // Create a size x size grid.
    Block **Blocks; 
};

// Setup board
void initBoard(GameBoard *board, int init_size);
void fillBoard(GameBoard *board);

// Block Matching algorithms
bool check_I_Match(GameBoard board, Block first, Block second);

bool check_L_Match(GameBoard board, Block first, Block second);

bool check_U_Top(GameBoard board, Block first, Block second);
bool check_U_Bottom(GameBoard board, Block first, Block second);
bool check_U_Left(GameBoard board, Block first, Block second);
bool check_U_Right(GameBoard board, Block first, Block second);
bool check_U_Match(GameBoard board, Block first, Block second);

bool check_Z_Match(GameBoard board, Block first, Block second);

// Visual Representation
void drawBlock(Block block);
void drawBoard(GameBoard board);

// Clean up board
void cleanBoard(GameBoard *board);