#pragma once


enum Block_mode
{
    NORMAL,
    LOCKED,
    EMPTY,
};

struct Block
{
    int x, y;   // the (x,y) coordinate of block inside the grid
    char value;             // the character represent the block
    bool isSelected;
    Block_mode mode;
};

void initBlock(Block &block, int x, int y, char c);
