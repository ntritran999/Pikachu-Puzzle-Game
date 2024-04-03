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
            initBLock(board->Blocks[i][j], j, i, block_value);
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
void drawBlock(Block block)
{
    int x = block.x * 7;
    int y = block.y * 5;

    gotoXY(30 + x, 3 + y);
    std::cout << " ----- \n";
    
    if (block.isSelected)
        setColor(WHITE, BLACK);
    else if (block.mode == LOCKED)
        setColor(GREEN, LIGHT_BLUE);
        
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

void drawSelectedEmptyBlock(Block block)
{
    int x = block.x * 7;
    int y = block.y * 5;

    gotoXY(30 + x, 3 + y);
    std::cout << "       \n";    

    setColor(WHITE, BLACK);

    gotoXY(30 + x, 4 + y);
    std::cout << "       \n";
    gotoXY(30 + x, 5 + y);
    std::cout << "       \n";
    gotoXY(30 + x, 6 + y);
    std::cout << "       \n";
    
    setDefaultColor();

    gotoXY(30 + x, 7 + y);
    std::cout << "       \n";
}

void drawBoard(GameBoard board)
{
    for (int i = 0; i < board.size; i++)
        for (int j = 0; j < board.size; j++)
        {
            Block block = board.Blocks[i][j];

            if (block.mode != EMPTY)
                drawBlock(block);
            else
                if (block.isSelected == true)
                    drawSelectedEmptyBlock(block);
        }
}

void drawILine(Block first, Block second)
{
    if (first.x == second.x)
    {
        int x = first.x * 7 + 3;
        int y1 = std::min(first.y, second.y) * 5 + 4;
        int y2 = std::max(first.y, second.y) * 5;
        gotoXY(30 + x, 3 + y1);
        std::cout << "^";
        //setColor(WHITE, BLACK);
        for (int i = y1 + 1; i < y2; i++)
        {
            Sleep(100);
            gotoXY(30 + x, 3 + i);
            std::cout << "|";
        }
        setDefaultColor();
        gotoXY(30 + x, 3 + y2);
        std::cout << "v";
    }

    if (first.y == second.y)
    {
        int y = first.y * 5 + 2;
        int x1 = std::min(first.x, second.x) * 7 + 6;
        int x2 = std::max(first.x, second.x) * 7;
        gotoXY(30 + x1, 3 + y);
        std::cout << "<";
        //setColor(WHITE, BLACK);
        for (int i = x1 + 1; i < x2; i++)
        {
            Sleep(100);
            gotoXY(30 + i, 3 + y);
            std::cout << "-";
        }
        setDefaultColor();
        gotoXY(30 + x2, 3 + y);
        std::cout << ">";
    }
}

void drawLLine(GameBoard board, Block first, Block second)
{
    Block tmp1 = board.Blocks[second.y][first.x];
    Block tmp2 = board.Blocks[first.y][second.x];

    if (tmp1.mode == EMPTY)
    {
        if (check_I_Match(board, first, tmp1) && check_I_Match(board, second, tmp1))
        {
            drawILine(first, tmp1);
            drawILine(second, tmp1);

            gotoXY(30 + tmp1.x * 7 + 3, 3 + tmp1.y * 5 + 2);
            if (first.x > second.x)
                std::cout << "O---";
            else
                std::cout << "---O";

            if (tmp1.y < first.y)
            {
                gotoXY(30 + tmp1.x * 7 + 3, 3 + tmp1.y * 5 + 3);
                std::cout << "|";
                gotoXY(30 + tmp1.x * 7 + 3, 3 + tmp1.y * 5 + 4);
                std::cout << "|";
            }
            else
            {
                gotoXY(30 + tmp1.x * 7 + 3, 3 + tmp1.y * 5 + 0);
                std::cout << "|";
                gotoXY(30 + tmp1.x * 7 + 3, 3 + tmp1.y * 5 + 1);
                std::cout << "|";                
            }
        }
    }
    
    else if (tmp2.mode == EMPTY)
        if (check_I_Match(board, first, tmp2) && check_I_Match(board, second, tmp2))
        {
            drawILine(first, tmp2);
            drawILine(second, tmp2);

            gotoXY(30 + tmp2.x * 7, 3 + tmp2.y * 5 + 2);
            if (first.x > second.x)
                std::cout << "O---";
            else
                std::cout << "---O";

            if (tmp2.y < second.y)
            {
                gotoXY(30 + tmp2.x * 7 + 3, 3 + tmp2.y * 5 + 3);
                std::cout << "|";
                gotoXY(30 + tmp2.x * 7 + 3, 3 + tmp2.y * 5 + 4);
                std::cout << "|";
            }
            else
            {
                gotoXY(30 + tmp2.x * 7 + 3, 3 + tmp2.y * 5 + 0);
                std::cout << "|";
                gotoXY(30 + tmp2.x * 7 + 3, 3 + tmp2.y * 5 + 1);
                std::cout << "|";                
            }
        }
}

void drawZLine(GameBoard board, Block first, Block second)
{
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
                temp1.x = std::min(first.x, second.x);
                temp2.x = std::max(first.x, second.x);
                drawILine(first,temp1);
                drawILine(temp1,temp2);
                drawILine(second,temp2);

                if (first.y > second.y)
                {
                    gotoXY(30 + temp1.x * 7, 3 + temp1.y * 5 + 2);
                    std::cout << "---";
                    gotoXY(30 + temp2.x * 7 + 4, 3 + temp2.y * 5 + 2);
                    std::cout << "---";
                }
                else
                {
                    gotoXY(30 + temp1.x * 7 + 3, 3 + temp1.y * 5 + 2);
                    std::cout << "---";
                    gotoXY(30 + temp2.x * 7, 3 + temp2.y * 5 + 2);
                    std::cout << "---";
                }

                for (int i = 3; i <= 4; i++)
                {
                    gotoXY(30 + temp2.x * 7 + 3, 3 + temp2.y * 5 + i);
                    std::cout << "|";
                }
                for (int i = 0; i <= 1; i++)
                {
                    gotoXY(30 + temp1.x * 7 + 3, 3 + temp1.y * 5 + i);
                    std::cout << "|";
                }
                break;
            }
        }
    }

    start = std::min(first.x, second.x);
    end = std::max(first.x, second.x);

    for (int col = start + 1; col < end; col++)
    {
        Block temp1 = board.Blocks[first.y][col];
        Block temp2 = board.Blocks[second.y][col];

        if (temp1.mode == EMPTY && temp2.mode == EMPTY)
        {
            temp1.y = std::min(first.y, second.y);
            temp2.y = std::max(first.y, second.y);
            // Check Z match by verifying three of its edges
            if (check_I_Match(board, first, temp1) &&
                check_I_Match(board, temp1, temp2) &&
                check_I_Match(board, second, temp2))
            {
                drawILine(first,temp1);
                drawILine(temp1,temp2);
                drawILine(second,temp2);

                if (first.x < second.x)
                {
                    gotoXY(30 + temp1.x * 7, 3 + temp1.y * 5 + 2);
                    std::cout << "---";
                    gotoXY(30 + temp2.x * 7 + 4, 3 + temp2.y * 5 + 2);
                    std::cout << "---";
                }
                else
                {
                    gotoXY(30 + temp1.x * 7 + 3, 3 + temp1.y * 5 + 2);
                    std::cout << "---";
                    gotoXY(30 + temp2.x * 7, 3 + temp2.y * 5 + 2);
                    std::cout << "---";
                }

                for (int i = 3; i <= 4; i++)
                {
                    gotoXY(30 + temp1.x * 7 + 3, 3 + temp1.y * 5 + i);
                    std::cout << "|";
                }
                for (int i = 0; i <= 1; i++)
                {
                    gotoXY(30 + temp2.x * 7 + 3, 3 + temp2.y * 5 + i);
                    std::cout << "|";
                }
                break;
            }
        }
    }
}

bool drawULineOutSide(int size, Block first, Block second)
{
    int x1 = std::min(first.x, second.x);
    int x2 = std::max(first.x, second.x);
    int y1 = std::min(first.y, second.y);
    int y2 = std::max(first.y, second.y);

    if ((first.y == second.y) && (first.y == 0 || first.y == size - 1))
    {
        char c = 'v';
        int new_y = first.y * 5, add = -1;
        if (first.y == size - 1)
        {
            new_y = first.y * 5 + 4;
            c = '^';
            add = 1;
        }
        gotoXY(33 + x1 * 7, 3 + new_y);
        std::cout << c;
        gotoXY(33 + x2 * 7, 3 + new_y);
        std::cout << c;

        int end = (x2 - x1) * 7 + 3;
        for (int i = 4; i <= end; i++)
        {
            gotoXY(30 + x1 * 7 + i, 3 + new_y + add);
            std::cout << "-";
        }
        return true;
    }

    if ((first.x == second.x) && (first.x == 0 || first.x == size - 1))
    {
        char c = '>';
        int new_x = first.x * 7, add = -2;
        if (first.x == size - 1)
        {
            new_x = first.y * 7 + 6;
            c = '<';
            add = 2;
        }
        gotoXY(30 + new_x, 5 + y1 * 5);
        std::cout << c;
        gotoXY(30 + new_x, 5 + y2 * 5);
        std::cout << c;

        int end = (y2 - y1) * 5 + 2;
        for (int i = 3; i <= end; i++)
        {
            gotoXY(30 + new_x + add, 3 + y1 * 5 + i);
            std::cout << "|";
        }
        return true;
    }
    return false;
}
void drawULine(GameBoard board, Block first, Block second)
{
    if (drawULineOutSide(board.size, first, second))
        return;
}

// Clean up board
void cleanBoard(GameBoard *board)
{
    for (int i = 0; i < board->size; i++)
        delete[] board->Blocks[i];

    delete[] board->Blocks;
}
