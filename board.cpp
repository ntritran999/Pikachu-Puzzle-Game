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
    int *pool = new int[pool_size]; // Create a pool of values to fill Board
    
    genRandoms(pool, pool_size);    // Get randoms values
    shuffleRandoms(pool, pool_size);    // Shuffle the pool

    for (int i = 0; i < board->size; i++)
    {
        for (int j = 0; j < board->size; j++)
        {
            char block_value = pool[pool_size - 1] + 'A';
            initBlock(board->Blocks[i][j], j, i, block_value); // Initialize each block in the board.
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
    if (first.x != second.x)
        check_vertical = false;
    else
    {
        // Check if there's any other block in the way.
        int start = std::min(first.y, second.y); 
        int end = std::max(first.y, second.y);

        // If there's a block that is not empty between first and second block, 
        // set check_vertical to false
        for (int row = start + 1; row < end; row++)
            if (board.Blocks[row][first.x].mode != EMPTY)
            {
                check_vertical = false;
                break;
            }
    }

    // Check horizontally
    if (first.y != second.y)
        check_horizontal = false;
    else
    {
        // Check if there's any other block in the way.
        int start = std::min(first.x, second.x); 
        int end = std::max(first.x, second.x);

        // If there's a block that is not empty between first and second block, 
        // set check_horizontal to false
        for (int col = start + 1; col < end; col++)
            if (board.Blocks[first.y][col].mode != EMPTY)
            {
                check_horizontal = false;
                break;
            }
    }

    return check_vertical || check_horizontal;
}

bool check_L_Match(GameBoard board, Block first, Block second) 
{
    bool flag1 = false;
    bool flag2 = false;

    // Find the position of the other two corners of the L shape 
    Block temp1 = board.Blocks[second.y][first.x];
    Block temp2 = board.Blocks[first.y][second.x];
    
    if (temp1.mode == EMPTY)
        // Check L match by verifying both of its edges
        flag1 = check_I_Match(board, first, temp1) && check_I_Match(board, second, temp1);
    
    if (temp2.mode == EMPTY)
        // Likewise, check L match by verifying the edges.
        flag2 = check_I_Match(board, first, temp2) && check_I_Match(board, second, temp2);
    
    return (flag1 || flag2);
}

bool check_Z_Match(GameBoard board, Block first, Block second) {

    bool check_vertical = false;
    bool check_horizontal = false;

    // Check vertically
  
    int start = std::min(first.y, second.y);
    int end = std::max(first.y, second.y);

    for (int row = start + 1; row < end; row++) {

        Block temp1 = board.Blocks[row][first.x];
        Block temp2 = board.Blocks[row][second.x];

        if (temp1.mode == EMPTY && temp2.mode == EMPTY) 
        {
            // Check Z match by verifying three of its edges
            if (check_I_Match(board, first, temp1) &&
                check_I_Match(board, temp1, temp2) &&
                check_I_Match(board, second, temp2))
            {
                check_vertical = true;
                break;
            }
        }
    }

    // Check horizontally

    start = std::min(first.x, second.x);
    end = std::max(first.x, second.x);

    for (int col = start + 1; col < end; col++)
    {
        Block temp1 = board.Blocks[first.y][col];
        Block temp2 = board.Blocks[second.y][col];

        if (temp1.mode == EMPTY && temp2.mode == EMPTY)
        {
            // Check Z match by verifying three of its edges
            if (check_I_Match(board, first, temp1) &&
                check_I_Match(board, temp1, temp2) &&
                check_I_Match(board, second, temp2))
            {
                check_horizontal = true;
                break;
            }
        }
    }
    return check_vertical || check_horizontal;
}


bool check_U_Horizontal(GameBoard board, Block first, Block second)
{
    // First check the case where two blocks are the edge of the board.
    if ((first.y == second.y) && (first.y == 0 || first.y == board.size - 1))
        return true;

    Block tmp1, tmp2;
    // Iterate from first row to last row.
    for (int i = 0; i < board.size; i++)
    {
        tmp1 = board.Blocks[i][first.x];
        tmp2 = board.Blocks[i][second.x];

        // For the first and last row, check for clear path between first block and tmp1 block
        // and check for clear path between second block and tmp2 block.
        if ((i == 0 || i == board.size - 1) && check_I_Match(board, first, tmp1) && check_I_Match(board, second, tmp2))
        {
            // If the first block or the second block is on the first or last row
            if ((i == first.y || i == second.y))
            {
                // If the tmp1 block and the first block are on the same row
                // and if the tmp1 block is empty, return true
                if (tmp1.y != first.y && tmp1.mode == EMPTY)
                    return true;
                // If the tmp2 block and the second block are on the same row
                // and if the tmp2 block is empty, return true
                if (tmp2.y != second.y && tmp2.mode == EMPTY)
                    return true;
            }
            else
            {
                // If the first or the second block not on first or last row
                // check to see if both tmp blocks are empty
                if (tmp1.mode == EMPTY && tmp2.mode == EMPTY)
                    return true;
            }

        }
        else
        {
            // For other rows, check for clear path between tmp1 and tmp2 block,
            // as well as clear path between first and tmp1 and between second and tmp2.
            if (check_I_Match(board, first, tmp1) && check_I_Match(board, second, tmp2)
                && check_I_Match(board, tmp1, tmp2)
                && (tmp1.mode == EMPTY && tmp2.mode == EMPTY))
                    return true;
        }
    }
    return false;
}

bool check_U_Vertical(GameBoard board, Block first, Block second)
{
    // First check the case where two blocks are the edge of the board.
    if ((first.x == second.x) && (first.x == 0 || first.x == board.size - 1))
        return true;

    Block tmp1, tmp2;
    // Iterate from first column to last column.
    for (int i = 0; i < board.size; i++)
    {
        tmp1 = board.Blocks[first.y][i];
        tmp2 = board.Blocks[second.y][i];

        // For the first and last column, check for clear path between first block and tmp1 block
        // and check for clear path between second block and tmp2 block.
        if ((i == 0 || i == board.size - 1) && check_I_Match(board, first, tmp1) && check_I_Match(board, second, tmp2))
        {
            // If the first block or the second block is on the first or last column
            if ((i == first.x || i == second.x))
            {
                // If the tmp1 block and the first block are on the same column
                // and if the tmp1 block is empty, return true
                if (tmp1.x != first.x && tmp1.mode == EMPTY)
                    return true;
                // If the tmp2 block and the second block are on the same column
                // and if the tmp2 block is empty, return true
                if (tmp2.x != second.x && tmp2.mode == EMPTY)
                    return true;
            }
            else
            {
                // If the first or the second block not on first or last column
                // check to see if both tmp blocks are empty
                if (tmp1.mode == EMPTY && tmp2.mode == EMPTY)
                    return true;
            }

        }
        else
        {
            // For other rows, check for clear path between tmp1 and tmp2 block,
            // as well as clear path between first and tmp1 and between second and tmp2.
            if (check_I_Match(board, first, tmp1) && check_I_Match(board, second, tmp2)
                && check_I_Match(board, tmp1, tmp2)
                && (tmp1.mode == EMPTY && tmp2.mode == EMPTY))
                    return true;
        }
    }
    return false;
}

bool check_U_Match(GameBoard board, Block first, Block second)
{
    return check_U_Horizontal(board, first, second) || check_U_Vertical(board, first, second);
}

// Visualize
void drawBlock(Block block, int background, int text)
{
    // Get the middle coordinate of the block
    int x = block.x * 7;
    int y = block.y * 5;

    // Move the cursor
    gotoXY(30 + x, 3 + y);
    std::cout << " ----- \n";
    
    setColor(background, text);

    // Move the cursor again    
    gotoXY(30 + x, 4 + y);
    std::cout << "|     |\n";
    gotoXY(30 + x, 5 + y);
    std::cout << "|  " << block.value << "  |\n";
    gotoXY(30 + x, 6 + y);
    std::cout << "|     |\n";
   
    setDefaultColor();
   
    gotoXY(30 + x, 7 + y);
    std::cout << " ----- \n";
}


void drawBoard(GameBoard board)
{
    // Iterate through each block and draw them.
    for (int i = 0; i < board.size; i++)
        for (int j = 0; j < board.size; j++)
        {
            Block block = board.Blocks[i][j];

            if (block.mode != EMPTY)
            {
                // Not empty and got selected
                if (block.isSelected == true)
                    drawBlock(block, WHITE, BLACK);
                // Not empty and got locked
                else if (block.mode == LOCKED)
                    drawBlock(block, GREEN, LIGHT_BLUE);
                // Not empty
                else
                    drawBlock(block, BLACK, WHITE);
            }
            else
                // Empty and got selected
                if (block.isSelected == true)
                    drawBlock(block, WHITE, BLACK);
        }
}


void drawCenterDot(int x, int y)
{
    // Go to the center of the block and draw a dot.
    gotoXY(30 + x * 7 + 3, 3 + y * 5 + 2);
    std::cout << 'o';
}

void drawILine(int x1, int x2, int y1, int y2)
{
    // Using the same logic as in the check I Match function.
    if (x1 == x2)
    {
        int x = x1 * 7 + 3;
        int y_min = std::min(y1, y2) * 5 + 4;
        int y_max = std::max(y1, y2) * 5;
        
        gotoXY(30 + x, 3 + y_min);
        std::cout << "^";
        
        for (int i = y_min + 1; i < y_max; i++)
        {
            Sleep(100); // For animation
            gotoXY(30 + x, 3 + i);
            std::cout << "|";
        }
        
        gotoXY(30 + x, 3 + y_max);
        std::cout << "v";
        return;
    }

    if (y1 == y2)
    {
        int y = y1 * 5 + 2;
        int x_min = std::min(x1, x2) * 7 + 6;
        int x_max = std::max(x1, x2) * 7;

        gotoXY(30 + x_min, 3 + y);
        std::cout << "<";

        for (int i = x_min + 1; i < x_max; i++)
        {
            Sleep(100);
            gotoXY(30 + i, 3 + y);
            std::cout << "-";
        }
        
        gotoXY(30 + x_max, 3 + y);
        std::cout << ">";
        return;
    }
}

void drawLLine(GameBoard board, Block first, Block second)
{
    // Using the same logic as in the check L Match function.
    Block tmp1 = board.Blocks[second.y][first.x];
    Block tmp2 = board.Blocks[first.y][second.x];

    if (tmp1.mode == EMPTY && check_I_Match(board, first, tmp1) && check_I_Match(board, second, tmp1))
    {
        drawCenterDot(tmp1.x, tmp1.y);
        drawILine(first.x, tmp1.x, first.y, tmp1.y);
        drawILine(second.x, tmp1.x, second.y, tmp1.y);
        return;
    }
    
    if (tmp2.mode == EMPTY && check_I_Match(board, first, tmp2) && check_I_Match(board, second, tmp2))
    {
        drawCenterDot(tmp2.x, tmp2.y);
        drawILine(first.x, tmp2.x, first.y, tmp2.y);
        drawILine(second.x, tmp2.x, second.y, tmp2.y);
        return;
    }
}

void drawZLine(GameBoard board, Block first, Block second)
{
    // Using the same logic as in the check Z Match function.
    int start, end;
    Block temp1, temp2;

    start = std::min(first.y, second.y);
    end = std::max(first.y, second.y);

    for (int row = start + 1; row < end; row++) {
        temp1 = board.Blocks[row][first.x];
        temp2 = board.Blocks[row][second.x];

        if (temp1.mode == EMPTY && temp2.mode == EMPTY) 
        {
            if (check_I_Match(board, first, temp1) &&
                check_I_Match(board, temp1, temp2) &&
                check_I_Match(board, second, temp2))
            {
                drawCenterDot(temp1.x, temp1.y);
                drawCenterDot(temp2.x, temp2.y);
                drawILine(temp1.x, temp2.x, temp1.y, temp2.y);
                drawILine(first.x, temp1.x, first.y, temp1.y);
                drawILine(second.x, temp2.x, second.y, temp2.y);
            }

            return;
        }
    }
    
    start = std::min(first.x, second.x);
    end = std::max(first.x, second.x);

    for (int col = start + 1; col < end; col++)
    {
        temp1 = board.Blocks[first.y][col];
        temp2 = board.Blocks[second.y][col];

        if (temp1.mode == EMPTY && temp2.mode == EMPTY)
        {
            if (check_I_Match(board, first, temp1) &&
                check_I_Match(board, temp1, temp2) &&
                check_I_Match(board, second, temp2))
            {
                drawCenterDot(temp1.x, temp1.y);
                drawCenterDot(temp2.x, temp2.y);
                drawILine(temp1.x, temp2.x, temp1.y, temp2.y);
                drawILine(first.x, temp1.x, first.y, temp1.y);
                drawILine(second.x, temp2.x, second.y, temp2.y);
            }

            return;
        }
    }
}

void drawULine(GameBoard board, Block first, Block second)
{
    // Using the same logic as in the check U Match function.
    if ((first.y == second.y) && (first.y == 0 || first.y == board.size - 1))
    {
        int add = 1;
        if (first.y == 0) add = -1;

        drawCenterDot(first.x, first.y + add);
        drawCenterDot(second.x, second.y + add);
        drawILine(first.x , first.x, first.y, first.y + add);
        drawILine(second.x , second.x, second.y, second.y + add);
        drawILine(first.x, second.x, first.y + add, second.y + add);
        return;
    }
    
    if ((first.x == second.x) && (first.x == 0 || first.x == board.size - 1))
    {
        int add = 1;
        if (first.x == 0) add = -1;

        drawCenterDot(first.x + add, first.y);
        drawCenterDot(second.x + add, second.y);
        drawILine(first.x , first.x + add, first.y, first.y);
        drawILine(second.x , second.x + add, second.y, second.y);
        drawILine(first.x + add, second.x + add, first.y, second.y);
        return;
    }
}

// Clean up board
void cleanBoard(GameBoard *board)
{
    for (int i = 0; i < board->size; i++)
        delete[] board->Blocks[i];

    delete[] board->Blocks;
}
