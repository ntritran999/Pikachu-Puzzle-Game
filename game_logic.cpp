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
    GameBoard board;
    int board_size = game->difficulty * 2 + 4;
    initBoard(&board, board_size);
    fillBoard(&board);
    board.Blocks[0][0].isSelected = true;

    int keyboard_input;
    int cursor_x = 0, cursor_y = 0;
    while (true)
    {
        resetOutofBound(cursor_x, cursor_y, board_size);        
        system("cls");

        drawBoard(board);

        keyboard_input = getch();

        if (keyboard_input == 0 || keyboard_input == 224)
        {
            switch (keyboard_input = getch())
            {
                case KEY_DOWN:
                    cursor_y++;
                    break;
                case KEY_UP:
                    cursor_y--;
                    break;
                case KEY_LEFT:
                    cursor_x--;
                    break;
                case KEY_RIGHT:
                    cursor_x++;
                    break;
                default:
                    break;
            }
        }
        
        else if (keyboard_input == ESC)
            break;
    }
}

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

                    if (!check_I || !check_U || !check_L || !check_Z)
                        return false;
                }
        }
    }
    return true;
}
