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
    int *pool = new int[pool_size];
    
    genRandoms(pool, pool_size);
    shuffleRandoms(pool, pool_size);

    for (int i = 0; i < board->size; i++)
    {
        for (int j = 0; j < board->size; j++)
        {
            char block_value = pool[pool_size - 1] + 'A';
            initBlock(board->Blocks[i][j], j, i, block_value);
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
    if ((first.y == second.y) && (first.y == 0 || first.y == board.size - 1))
        return true;

    Block tmp1, tmp2;
    for (int i = 0; i < board.size; i++)
    {
        tmp1 = board.Blocks[i][first.x];
        tmp2 = board.Blocks[i][second.x];

        if ((i == 0 || i == board.size - 1) && check_I_Match(board, first, tmp1) && check_I_Match(board, second, tmp2))
        {
            if ((i == first.y || i == second.y))
            {
                if (tmp1.y != first.y && tmp1.mode == EMPTY)
                    return true;
                if (tmp2.y != second.y && tmp2.mode == EMPTY)
                    return true;
            }
            else
            {
                if (tmp1.mode == EMPTY && tmp2.mode == EMPTY)
                    return true;
            }

        }
        else
        {
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
    if ((first.x == second.x) && (first.x == 0 || first.x == board.size - 1))
        return true;

    Block tmp1, tmp2;
    for (int i = 0; i < board.size; i++)
    {
        tmp1 = board.Blocks[first.y][i];
        tmp2 = board.Blocks[second.y][i];

        if ((i == 0 || i == board.size - 1) && check_I_Match(board, first, tmp1) && check_I_Match(board, second, tmp2))
        {
            if ((i == first.x || i == second.x))
            {
                if (tmp1.x != first.x && tmp1.mode == EMPTY)
                    return true;
                if (tmp2.x != second.x && tmp2.mode == EMPTY)
                    return true;
            }
            else
            {
                if (tmp1.mode == EMPTY && tmp2.mode == EMPTY)
                    return true;
            }

        }
        else
        {
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
    int x = block.x * 7;
    int y = block.y * 5;

    gotoXY(30 + x, 3 + y);
    std::cout << " ----- \n";
    
    setColor(background, text);
        
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
    for (int i = 0; i < board.size; i++)
        for (int j = 0; j < board.size; j++)
        {
            Block block = board.Blocks[i][j];

            if (block.mode != EMPTY)
            {
                if (block.isSelected == true)
                    drawBlock(block, WHITE, BLACK);
                else if (block.mode == LOCKED)
                    drawBlock(block, GREEN, LIGHT_BLUE);
                else
                    drawBlock(block, BLACK, WHITE);
            }
            else
                if (block.isSelected == true)
                    drawBlock(block, WHITE, BLACK);
        }
}


void drawCenterDot(int x, int y)
{
    gotoXY(30 + x * 7 + 3, 3 + y * 5 + 2);
    std::cout << 'o';
}

void drawILine(int x1, int x2, int y1, int y2)
{
    if (x1 == x2)
    {
        int x = x1 * 7 + 3;
        int y_min = std::min(y1, y2) * 5 + 4;
        int y_max = std::max(y1, y2) * 5;
        
        gotoXY(30 + x, 3 + y_min);
        std::cout << "^";
        
        for (int i = y_min + 1; i < y_max; i++)
        {
            Sleep(100);
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

            drawCenterDot(temp1.x, temp1.y);
            drawCenterDot(temp2.x, temp2.y);
            drawILine(temp1.x, temp2.x, temp1.y, temp2.y);
            drawILine(first.x, temp1.x, first.y, temp1.y);
            drawILine(second.x, temp2.x, second.y, temp2.y);

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

            drawCenterDot(temp1.x, temp1.y);
            drawCenterDot(temp2.x, temp2.y);
            drawILine(temp1.x, temp2.x, temp1.y, temp2.y);
            drawILine(first.x, temp1.x, first.y, temp1.y);
            drawILine(second.x, temp2.x, second.y, temp2.y);

            return;
        }
    }
}

void drawULine(GameBoard board, Block first, Block second)
{
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
    
    Block tmp1, tmp2;
    for (int row = 0; row < board.size; row++)
    {
        tmp1 = board.Blocks[row][first.x];
        tmp2 = board.Blocks[row][second.x];
        
        if ((row == 0 || row == board.size - 1) && check_I_Match(board, first, tmp1) && check_I_Match(board, second, tmp2))
        {
            bool check1 = (row == first.x) || (row == second.x);
            bool check2 = ((tmp1.x != first.x && tmp1.mode == EMPTY) || (tmp2.x != second.x && tmp2.mode == EMPTY));
            bool check3 = (tmp1.mode == EMPTY && tmp2.mode == EMPTY);

            if ((check1 && check2) || (!check1 && check3))
            {
                int add = 1;
                if (row == 0) add = -1;

                drawCenterDot(tmp1.x, tmp1.y + add);
                drawCenterDot(tmp2.x, tmp2.y + add);
                drawILine(tmp1.x, tmp2.x, tmp1.y + add, tmp2.y + add);
                drawILine(first.x , tmp1.x, first.y, tmp1.y + add);
                drawILine(second.x , tmp2.x, second.y, tmp2.y + add);
                return;
            }
        }
        else
        {
            if (check_I_Match(board, first, tmp1) && check_I_Match(board, second, tmp2)
                && check_I_Match(board, tmp1, tmp2)
                && (tmp1.mode == EMPTY && tmp2.mode == EMPTY))
            {

                drawCenterDot(tmp1.x, tmp1.y);
                drawCenterDot(tmp2.x, tmp2.y);
                drawILine(tmp1.x , first.x, tmp1.y, first.y);
                drawILine(tmp2.x , second.x, tmp2.y, second.y);
                drawILine(tmp1.x, tmp2.x, tmp1.y, tmp2.y);
            }

            return;
        }
    }
    
    for (int col = 0; col < board.size; col++)
    {
        tmp1 = board.Blocks[first.y][col];
        tmp2 = board.Blocks[second.y][col];
        
        if ((col == 0 || col == board.size - 1) && check_I_Match(board, first, tmp1) && check_I_Match(board, second, tmp2))
        {
            bool check1 = (col == first.x) || (col == second.x); 
            bool check2 = ((tmp1.x != first.x && tmp1.mode == EMPTY) || (tmp2.x != second.x && tmp2.mode == EMPTY));
            bool check3 = (tmp1.mode == EMPTY) && (tmp2.mode == EMPTY);
            
            if ((check1 && check2) || (!check1 && check3))
            {
                int add = 1;
                if (col == 0) add = -1;

                drawCenterDot(tmp1.x + add, tmp1.y);
                drawCenterDot(tmp2.x + add, tmp2.y);
                drawILine(tmp1.x + add, tmp2.x + add, tmp1.y, tmp2.y);
                drawILine(first.x , tmp1.x + add, first.y, tmp1.y);
                drawILine(second.x , tmp2.x + add, second.y, tmp2.y);
                return;
            }
        }
        else
        {
            if (check_I_Match(board, first, tmp1) && check_I_Match(board, second, tmp2)
                && check_I_Match(board, tmp1, tmp2)
                && (tmp1.mode == EMPTY && tmp2.mode == EMPTY))
            {
                drawCenterDot(tmp1.x, tmp1.y);
                drawCenterDot(tmp2.x, tmp2.y);
                drawILine(tmp1.x, tmp2.x, tmp1.y, tmp2.y);
                drawILine(tmp1.x , first.x, tmp1.y, first.y);
                drawILine(tmp2.x , second.x, tmp2.y, second.y);
            }
            return;
        }
    }
}

// Clean up board
void cleanBoard(GameBoard *board)
{
    for (int i = 0; i < board->size; i++)
        delete[] board->Blocks[i];

    delete[] board->Blocks;
}
