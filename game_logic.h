#pragma once

#include <chrono>
#include <thread>
#include <conio.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "board.h"

extern bool stopTimer;

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
    bool gameFinished;
};

// Game stage
void startGame(GameInfo *game);
void loadGame(GameInfo *game, int duration);
void saveGame(std::string message);

// Draw background
void drawBackground(GameDifficulty difficulty);

// Movements
void resetOutofBound(int &x, int &y, int size);
void moveCursor(int c, int &x, int &y, Block *block);
void lockCursor(int &n, Block *block);
void unlockCursor(int &n, Block *block);

// Scoring
void drawScoreBoard(int score);
bool scoreIMatch(int &score, GameBoard board, Block *first, Block *second);
bool scoreLMatch(int &score, GameBoard board, Block *first, Block *second);
bool scoreZMatch(int &score, GameBoard board, Block *first, Block *second);
bool scoreUMatch(int &score, GameBoard board, Block *first, Block *second);

// Check end game
bool checkEmptyBoard(GameBoard board);
bool checkRemainPairs(GameBoard board);

// Helping tool
void moveSuggestion(GameBoard board);

//Timer
void countDownTimer(int *duration);
void drawTimer(int duration);