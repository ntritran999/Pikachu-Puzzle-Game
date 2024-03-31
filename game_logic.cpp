#include "game_logic.h"

void startGame()
{
    GameInfo game;
    int option;
    
    while (true)
    {
        system("cls");
        std::cout << "Choose difficulty: \n";
        std::cout << "1.Easy\n";
        std::cout << "2.Medium\n";
        std::cout << "3.Hard\n";
        std::cout << "\n[PRESS ESC KEY TO GO BACK TO MAIN MENU]\n";

        option = getch();

        if (option == '1' || option == '2' || option == '3')
        {
            // Initialize the game's information
            game.score = 0;
            if (option == '1')
                game.difficulty = EASY;
            else if (option == '2')
                game.difficulty = MEDIUM;
            else
                game.difficulty = HARD;
            
            // Loading
            system("cls");
            std::cout << "Loading game...\n";
            Sleep(2000);

            loadGame(&game);
        }

        else if (option == ESC)
        {
            system("cls");
            std::cout << "Going back to main menu...\n";
            Sleep(2000); // delay 2 seconds
            break;
        }

        else
        {
            std::cout << "Please choose the correct option!\n";
            Sleep(1000); //delay 1 second
        }
    }
}

void loadGame(GameInfo *game)
{
    // Initialize board
    srand(time(0));
    GameBoard board;
    int board_size = game->difficulty * 2 + 4;
    initBoard(&board, board_size);
    fillBoard(&board);

    // Game score
    int score = game->score;

    // Initialize cursor
    int cursor_x = 0, cursor_y = 0;
    Block *first = nullptr, *second = nullptr, *current;
    int nLocks = 0;
    int c;

    // Main game loop
    while (true)
    {
        resetOutofBound(cursor_x, cursor_y, board_size);        
        current = &board.Blocks[cursor_y][cursor_x];
        current->isSelected = true;

        system("cls");
        drawBoard(board);

        c = getch();

        if (c == 0 || c == 224)
            moveCursor(c, cursor_x, cursor_y, current);

        else if (c == SPACE_KEY)
        {
            lockCursor(nLocks, current);

            if (nLocks == 1)
                first = current;
            else if (nLocks == 2)
                second = current;
        }
        
        else if (c == 'u')
            unlockCursor(nLocks, current);

        // Confirm blocks
        else if (c == '\r')
        {
            if (first != nullptr && second != nullptr)
            {
                //Start scoring after confirming
                if (scoreIMatch(score, board, first, second) ||
                    scoreLMatch(score, board, first, second) ||
                    scoreZMatch(score, board, first, second) ||
                    scoreUMatch(score, board, first, second))
                {
                    nLocks = 0;
                }
                Sleep(500);
            }
        }
        
        else if (c == ESC)
            break;
        
        if (checkEmptyBoard(board))
        {
            break;
        }
    }
}

// Movements
void resetOutofBound(int &x, int &y, int size)
{
    if (x < 0)
        x = 0;
    if (x >= size)
        x = size - 1;

    if (y < 0)
        y = 0;
    if (y >= size)
        y = size - 1;
}

void moveCursor(int c, int &x, int &y, Block *block)
{
    switch (c = getch())
    {
        case KEY_DOWN:
            block->isSelected = false;
            y++;
            break;
        case KEY_UP:
            block->isSelected = false;
            y--;
            break;
        case KEY_LEFT:
            block->isSelected = false;
            x--;
            break;
        case KEY_RIGHT:
            block->isSelected = false;
            x++;
            break;
        default:
            break;
    }
}

void lockCursor(int &n, Block *block)
{
    if (n != 2)
    {
        block->mode = LOCKED;
        n++;
    }
}

void unlockCursor(int &n, Block *block)
{
    if (block->mode == LOCKED)
    {
        block->mode = NORMAL;
        n--;
    }
}

// Scoring
bool scoreIMatch(int &score, GameBoard board, Block *first, Block *second)
{
    if (check_I_Match(board, *first, *second) && first->value == second->value)
    {
        first->mode = second->mode = EMPTY;
        score++;
        return true;
    }

    return false;
}

bool scoreLMatch(int &score, GameBoard board, Block *first, Block *second)
{
    if (check_L_Match(board, *first, *second) && first->value == second->value)
    {
        first->mode = second->mode = EMPTY;
        score++;
        return true;
    }

    return false;
}

bool scoreZMatch(int &score, GameBoard board, Block *first, Block *second)
{
    if (check_Z_Match(board, *first, *second) && first->value == second->value)
    {
        first->mode = second->mode = EMPTY;
        score++;
        return true;
    }

    return false;
}

bool scoreUMatch(int &score, GameBoard board, Block *first, Block *second)
{
    if (check_U_Match(board, *first, *second) && first->value == second->value)
    {
        first->mode = second->mode = EMPTY;
        return true;
    }

    return false;
}

bool checkEmptyBoard(GameBoard board)
{
    for (int i = 0; i < board.size; i++)
        for (int j = 0; j < board.size; j++)
            if (board.Blocks[i][j].mode != EMPTY)
                return false;
    return true;
}

bool checkRemainPairs(GameBoard board)
{
    Block first, second;
    for (int i = 0; i < board.size; i++)
    {
        for (int j = 0; i < board.size; j++)
        {
            first = board.Blocks[i][j];

            if (first.mode == EMPTY) 
                continue;

            for (int r = i; r < board.size; r++)
                for (int c = j; c < board.size; c++)
                {
                    second = board.Blocks[r][c];

                    if (second.mode == EMPTY)
                        continue;

                    bool check_I = check_I_Match(board, first, second);
                    bool check_U = check_U_Match(board, first, second);
                    bool check_L = check_L_Match(board, first, second);
                    bool check_Z = check_Z_Match(board, first, second);

                    if (!check_I && !check_U && !check_L && !check_Z)
                        return false;
                }
        }
    }
    return true;
}
