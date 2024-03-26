#include "block.h"

void initBLock(Block &block, int x, int y, char c)
{
    block.x = x;
    block.y = y;
    block.value = c;
    block.mode = NORMAL;
}