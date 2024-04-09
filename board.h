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

bool check_Z_Match(GameBoard board, Block first, Block second);

bool check_U_Horizontal(GameBoard board, Block first, Block second);
bool check_U_Vertical(GameBoard board, Block first, Block second);
bool check_U_Match(GameBoard board, Block first, Block second);

// Visual Representation
void drawBlock(Block block, int background, int text);
void drawBoard(GameBoard board);

void drawCenterDot(int x, int y);
void drawILine(int x1, int x2, int y1, int y2);
void drawLLine(GameBoard board, Block first, Block second);
void drawZLine(GameBoard board, Block first, Block second);
void drawULine(GameBoard board, Block first, Block second);

// Clean up board
void cleanBoard(GameBoard *board);