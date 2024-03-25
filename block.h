#pragma once


enum Block_mode
{
    NORMAL,
    LOCKED,
    EMPTY,
};

struct Block
{
    int coord_x, coord_y;   // the (x,y) coordinate of block inside the grid
    char value;             // the character represent the block
    Block_mode mode;
};

void initBLock(Block &block, int x, int y, char c);
