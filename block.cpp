#include "block.h"

void initBLock(Block &block, int x, int y, char c)
{
    block.coord_x = x;
    block.coord_y = y;
    block.value = c;
    block.mode = NORMAL;
}