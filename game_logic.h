#pragma once

#include <iostream>
#include <conio.h>

#include "board.h"

enum GameDifficulty
{
    EASY,
    MEDIUM,
    HARD,
};

struct GameInfo
{
    GameDifficulty difficulty;
    int score;
    bool gameOver;
};

// Game stage
void startGame();
void loadGame(GameInfo *game);
void exitGame();
void saveGame();

// Movements
void resetOutofBound(int &x, int &y, int size);
void moveCursor(int c, int &x, int &y, Block *block);
void lockCursor(int &n, Block *block);
void unlockCursor(int &n, Block *block);

// Check end game
bool checkEmptyBoard(GameBoard board);
bool checkRemainPairs(GameBoard board);

void moveSuggestion();