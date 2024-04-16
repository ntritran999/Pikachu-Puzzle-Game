#include "game_logic.h"
#include "stage_difficulty_increase.h"

bool stopTimer; // Use to stop the timer

void startGame(GameInfo *game)
{
    // Initialize game information in case the player wants to quit
    // right after entering name.
    game->difficulty = UNKNOWN;
    game->gameFinished = false;
    game->score = -1;

    int option;

    while (true)
    {
        // Printing out the choices
        system("cls");
        setColor(BLACK, LIGHT_AQUA);
        std::cout << "Choose difficulty: \n";
        setColor(BLACK, GREEN);
        std::cout << "1.Easy\n";
        setColor(BLACK, YELLOW);
        std::cout << "2.Medium\n";
        setColor(BLACK, RED);
        std::cout << "3.Hard\n";
        setColor(BLACK, LIGHT_RED);
        std::cout << "4.Challenge\n";
        setDefaultColor();
        std::cout << "\n[PRESS ESC KEY TO GO BACK TO MAIN MENU]\n";

        // Get the user keyboard value.
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

        else if (option == '4')
        {
            int duration = 120;
            game->score = 0;
            game->difficulty = CHALLENGE;

            // Loading
            system("cls");
            std::cout << "Loading challenge mode...\n";
            Sleep(2000);

            loadChallengeMode(*game, duration);
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
    // Seed for random
    srand(time(0));

    // Initialize board
    GameBoard board;
    int board_size = game->difficulty * 2 + 4;
    initBoard(&board, board_size);
    fillBoard(&board);

    // Initialize game information
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
        // Reset the cursor
        resetOutofBound(cursor_x, cursor_y, board_size);        
        current = &board.Blocks[cursor_y][cursor_x];
        current->isSelected = true;

        // Drawing
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
        
        // Unlocking selected blocks
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
                    bool isScored = false;
                    if (scoreIMatch(score, board, first, second))
                            isScored = true;
                    else if (scoreLMatch(score, board, first, second))
                            isScored = true;
                    else if (scoreZMatch(score, board, first, second))
                            isScored = true;
                    else if (scoreUMatch(score, board, first, second))
                            isScored = true;
                    
                    else
                    {
                        // If the selected blocks do not match, unlock both of them.
                        unlockCursor(nLocks, first);
                        unlockCursor(nLocks, second);
                        first = second = nullptr;
                    }

                    if (isScored)
                    {
                        first->mode = second->mode = EMPTY;
                        first->value = second->value = ' '; // Set the value to blank for later drawing.
                        nLocks = 0;
                    }
                }    
                Sleep(500);
            }
        }
        
        // Quiting the game
        else if (c == ESC)
        {
            cleanBoard(&board);
            game->score = score;
            stopTimer = true;
            saveGame("Quiting game!");
            break;
        }

        // Move suggestion
        else if (c == 'h')
        {
            Block f1, f2;
            moveSuggestion(board, f1, f2);
            // Highlighting the suggested blocks.
            drawBlock(f1, PURPLE, WHITE);
            drawBlock(f2, PURPLE, WHITE);
            Sleep(1500); // Delay for 1,5 seconds for the player to see.
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

// Print out message when the game is done.
void saveGame(std::string message)
{
    system("cls");
    setColor(BLACK, YELLOW);
    std::cout << message << "\n";
    setDefaultColor();
    Sleep(500);
    std::cout << "Saving game...\n";
    // Play sound
    playSound(ENDGAME_SOUND);
    Sleep(1000);
}

////////////////////////////////////////////////////////
//Draw background
void drawBackground(GameDifficulty difficulty)
{
    std::string file_name = "./images/";
    file_name += difficulty + "1";
    file_name += ".txt";

    std::ifstream bg_file(file_name);
    
    if (!bg_file)
    {
        std::cout << "Cannot open file!\n";
        return;
    }
    else
    {
        // Print out the background image
        std::string line;
        int i = 0;
        while(std::getline(bg_file, line))
        {
            gotoXY(30, 3 + i);
            std::cout << line << "\n";
            i++;
        }

        bg_file.close();
    }
}

// Movements
void resetOutofBound(int &x, int &y, int size)
{
    if (x < 0) x = 0;
    
    if (y < 0) y = 0;

    if (x >= size) x = size - 1;

    if (y >= size) y = size - 1;
}

void moveCursor(int c, int &x, int &y, Block *block)
{
    c = getch();
    if (c == KEY_DOWN || c == KEY_UP || c == KEY_RIGHT || c == KEY_LEFT)
    {
        playSound(MOVE_SOUND);
        block->isSelected = false;
    }

    switch (c)
    {
        case KEY_DOWN:
            y++;
            break;
        case KEY_UP:
            y--;
            break;
        case KEY_LEFT:
            x--;
            break;
        case KEY_RIGHT:
            x++;
            break;
        default:
            playSound(ERROR_SOUND);
            break;
    }
}

void lockCursor(int &n, Block *block)
{
    if (n != 2 && block->mode != LOCKED) // Avoiding locking more than two blocks 
    {                                    // and locking the same block twice.
        playSound(LOCK_SOUND);
        block->mode = LOCKED;
        n++;
    }
}

void unlockCursor(int &n, Block *block)
{
    if (block->mode == LOCKED)
    {
        playSound(UNLOCK_SOUND);
        block->mode = NORMAL;
        n--;
    }
}

// Scoring
void drawScoreBoard(int score)
{
    gotoXY(0,1);
    setColor(BLACK, LIGHT_YELLOW);
    std::cout << "SCORE: " << score;
    setDefaultColor();
}

bool scoreIMatch(int &score, GameBoard board, Block *first, Block *second)
{
    // Check if the two values are the same and it's a I matching pattern.
    if (first->value == second->value && check_I_Match(board, *first, *second))
    {
        drawILine(first->x, second->x, first->y, second->y);
        score++;
        return true;
    }

    return false;
}

bool scoreLMatch(int &score, GameBoard board, Block *first, Block *second)
{
    // Check if the two values are the same and it's a L matching pattern.
    if (first->value == second->value && check_L_Match(board, *first, *second))
    {
        drawLLine(board, *first, *second);
        score += 2;
        return true;
    }

    return false;
}

bool scoreZMatch(int &score, GameBoard board, Block *first, Block *second)
{
    // Check if the two values are the same and it's a Z matching pattern.
    if (first->value == second->value && check_Z_Match(board, *first, *second))
    {
        drawZLine(board, *first, *second);
        score += 3;
        return true;
    }

    return false;
}

bool scoreUMatch(int &score, GameBoard board, Block *first, Block *second)
{
    // Check if the two values are the same and it's a U matching pattern.
    if (first->value == second->value && check_U_Match(board, *first, *second))
    {
        drawULine(board, *first, *second);
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

            // If first is empty, skip to next iteration.
            if (first.mode == EMPTY) 
                continue;

            for (int r = 0; r < board.size; r++)
                for (int c = 0; c < board.size; c++)
                {
                    // If first and second are the same block, skip to next iteration.
                    if (i == r && j == c)
                        continue;

                    second = board.Blocks[r][c];
                    
                    // If second is empty, skip to next iteration.
                    // or two values are different, skip to next iteration.
                    if (second.mode == EMPTY || (first.value != second.value))
                        continue;
                    
                    if (check_I_Match(board, first, second) 
                        || check_L_Match(board, first, second) 
                        || check_Z_Match(board, first, second)
                        || check_U_Match(board, first, second)) 
                        return true;
                }
        }
    }
    return false;
}

// Move suggestion
void moveSuggestion(GameBoard board, Block &found1, Block &found2)
{
    // Use the logic as the checkRemainPairs function.
    // The only difference is that we return a pair of valid blocks before exiting.
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

                    if (second.mode == EMPTY || (first.value != second.value))
                        continue;
                    
                    if (check_I_Match(board, first, second) 
                        || check_L_Match(board, first, second) 
                        || check_Z_Match(board, first, second)
                        || check_U_Match(board, first, second)) 
                    {
                        found1 = first;
                        found2 = second;
                        return;
                    }
                }
        }
    }
}

// Timer
void countDownTimer(int *duration) 
{
    // If time is not up yet
    while (*duration >= 0 && !stopTimer)
    {
        // Draw the time UI.
        gotoXY(0,0);
        if (*duration > 10)
            setColor(BLACK, LIGHT_GREEN);
        else if (*duration > 5)
            setColor(BLACK, LIGHT_YELLOW);
        else
            setColor(BLACK, LIGHT_RED);
        std::cout << "TIME: " << *duration;

        setDefaultColor();
        (*duration)--;

        // Delay the thread for 1 second.
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}