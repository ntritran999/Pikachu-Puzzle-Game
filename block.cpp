#include "block.h"

void initBlock(Block &block, int x, int y, char c)
{
    block.x = x;
    block.y = y;
    block.value = c;
    block.isSelected = false;
    block.mode = NORMAL;
}