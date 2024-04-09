#include "game_logic.h"

bool stopTimer;

void startGame(GameInfo *game)
{
    int option;

    while (true)
    {
        system("cls");
        setColor(BLACK, LIGHT_AQUA);
        std::cout << "Choose difficulty: \n";
        setColor(BLACK, GREEN);
        std::cout << "1.Easy\n";
        setColor(BLACK, YELLOW);
        std::cout << "2.Medium\n";
        setColor(BLACK, RED);
        std::cout << "3.Hard\n";
        setDefaultColor();
        std::cout << "\n[PRESS ESC KEY TO GO BACK TO MAIN MENU]\n";

        option = getch();

        if (option == '1' || option == '2' || option == '3')
        {
            // Initialize the game's information and time
            int duration = (option - '0') * 60;
            game->score = 0;
            if (option == '1')
                game->difficulty = EASY;
            else if (option == '2')
                game->difficulty = MEDIUM;
            else
                game->difficulty = HARD;
            
            // Loading
            system("cls");
            std::cout << "Loading game...\n";
            Sleep(2000);

            loadGame(game, duration);
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

void loadGame(GameInfo *game, int duration)
{
    // Initialize board
    srand(time(0));
    GameBoard board;
    int board_size = game->difficulty * 2 + 4;
    initBoard(&board, board_size);
    fillBoard(&board);

    // Game score
    int score = game->score;
    game->gameFinished = false;

    // Initialize time
    std::thread timer(countDownTimer, &duration);
    stopTimer = false;

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
        drawBackground(game->difficulty);
        drawBoard(board);
        drawScoreBoard(score);

        c = getch();

        if (c == 0 || c == 224)
            moveCursor(c, cursor_x, cursor_y, current);

        else if (c == SPACE_KEY)
        {
            if (current->mode != EMPTY)
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
                if (first->mode != EMPTY && second->mode != EMPTY)
                {
                    //Start scoring after confirming
                    if (scoreIMatch(score, board, first, second))
                            nLocks = 0;
                    else if (scoreLMatch(score, board, first, second))
                            nLocks = 0;
                    else if (scoreZMatch(score, board, first, second))
                            nLocks = 0;
                    else if (scoreUMatch(score, board, first, second))
                            nLocks = 0;
                    
                    else
                    {
                        unlockCursor(nLocks, first);
                        unlockCursor(nLocks, second);
                        first = second = nullptr;
                    }
                }    
                Sleep(500);
            }
        }
        
        else if (c == ESC)
        {
            cleanBoard(&board);
            game->score = score;
            stopTimer = true;
            saveGame("Quiting game!");
            break;
        }

        else if (c == 'h')
        {
            moveSuggestion(board);
            Sleep(5000);
        }
        
        if (checkEmptyBoard(board))
        {
            cleanBoard(&board);
            game->score = score;
            game->gameFinished = true;
            stopTimer = true;
            saveGame("Board is empty!");
            break;
        }

        if (!checkRemainPairs(board))
        {
            cleanBoard(&board);
            game->score = score;
            game->gameFinished = true;
            stopTimer = true;
            saveGame("No valid pairs remain!");
            break;
        }

        if (duration <= 0)
        {
            cleanBoard(&board);
            game->score = score;
            saveGame("Time up! You lost!");
            break;
        }
    }

    timer.join();
}

void saveGame(std::string message)
{
    system("cls");
    setColor(BLACK, YELLOW);
    std::cout << message << "\n";
    setDefaultColor();
    Sleep(500);
    std::cout << "Saving game...\n";
    // Play sound

    Sleep(1000);
}

////////////////////////////////////////////////////////
//Draw background
void drawBackground(GameDifficulty difficulty)
{
    std::string file_name;
    if (difficulty == EASY)
        file_name = R"(.\images\1.txt)";
    else if (difficulty == MEDIUM)
        file_name = R"(.\images\2.txt)";
    else
        file_name = R"(.\images\3.txt)";

    std::ifstream bg_file(file_name);
    
    if (!bg_file.is_open())
    {
        std::cout << "Cannot open file!\n";
        return;
    }
    else
    {
        std::vector <std::string> background;
        std::string line;
        while (std::getline(bg_file, line))
            background.push_back(line + '\n');
        bg_file.close();
        
        for (int i = 0; i < background.size(); i++)
        {
            gotoXY(30, 3 + i);
            std::cout << background[i];
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
    if (n != 2 && block->mode != LOCKED)
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
void drawScoreBoard(int score)
{
    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);

    int max_len = desktop.right / 8;

    gotoXY(max_len - 50, 2);
    std::cout << "+------------------+\n";
    gotoXY(max_len - 50, 3);
    std::cout << "|                  |\n";
    gotoXY(max_len - 50, 4);
    std::cout << "|    SCORE: " << score << "    ";
    if (score < 10)
        std::cout << "  |\n";
    else if (score < 100)
        std::cout << " |\n";
    else if (score < 1000)
        std::cout << "|\n";
    gotoXY(max_len - 50, 5);
    std::cout << "|                  |\n";
    gotoXY(max_len - 50, 6);
    std::cout << "+------------------+\n";
}

bool scoreIMatch(int &score, GameBoard board, Block *first, Block *second)
{
    if (first->value == second->value && check_I_Match(board, *first, *second))
    {
        drawILine(first->x, second->x, first->y, second->y);
        first->mode = second->mode = EMPTY;
        score++;
        return true;
    }

    return false;
}

bool scoreLMatch(int &score, GameBoard board, Block *first, Block *second)
{
    if (first->value == second->value && check_L_Match(board, *first, *second))
    {
        if (first->y < second->y)
            drawLLine(board, *second, *first);
        else
            drawLLine(board, *first, *second);
        first->mode = second->mode = EMPTY;
        score += 2;
        return true;
    }

    return false;
}

bool scoreZMatch(int &score, GameBoard board, Block *first, Block *second)
{
    if (first->value == second->value && check_Z_Match(board, *first, *second))
    {
        drawZLine(board, *first, *second);
        first->mode = second->mode = EMPTY;
        score += 3;
        return true;
    }

    return false;
}

bool scoreUMatch(int &score, GameBoard board, Block *first, Block *second)
{
    if (first->value == second->value && check_U_Match(board, *first, *second))
    {
        drawULine(board, *first, *second);
        first->mode = second->mode = EMPTY;
        score += 3;
        return true;
    }

    return false;
}

// Check end game
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
        for (int j = 0; j < board.size; j++)
        {
            first = board.Blocks[i][j];

            if (first.mode == EMPTY) 
                continue;

            for (int r = 0; r < board.size; r++)
                for (int c = 0; c < board.size; c++)
                {
                    if (i == r && j == c)
                        continue;

                    second = board.Blocks[r][c];

                    if (second.mode == EMPTY)
                        continue;

                    if (first.value != second.value)
                        continue;

                    bool check_I = check_I_Match(board, first, second);
                    bool check_U = check_U_Match(board, first, second);
                    bool check_L = check_L_Match(board, first, second);
                    bool check_Z = check_Z_Match(board, first, second);
                    
                    if (check_I || check_U || check_L || check_Z)
                        return true;
                }
        }
    }

    return false;
}

// Move suggestion
void moveSuggestion(GameBoard board)
{

}

// Timer
void countDownTimer(int *duration) 
{
    while (*duration >= 0 && !stopTimer)
    {
        drawTimer(*duration);
        (*duration)--;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void drawTimer(int duration)
{
    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);

    int max_len = desktop.right / 8;

    gotoXY(max_len - 50, 7);
    std::cout << "|                  |\n";
    gotoXY(max_len - 50, 8);
    std::cout << "|    TIME: " << duration << "     ";
    if (duration < 10)
        std::cout << "  |\n";
    else if (duration < 100)
        std::cout << " |\n";
    else if (duration < 1000)
        std::cout << "|\n";
    gotoXY(max_len - 50, 9);
    std::cout << "|                  |\n";
    gotoXY(max_len - 50, 10);
    std::cout << "+------------------+\n";
}