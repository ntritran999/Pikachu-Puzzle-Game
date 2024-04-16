#pragma once

#include <cstring>

#include "window_handler.h"
#include "game_logic.h"

struct PlayerInfo {
	char playerName[256];
	int score;
	char mode[256];
	char status[256];
};

void printLogo();
void printGuide();

// Player related
void savePlayerInfo(const PlayerInfo player);
void printLeaderBoard();

bool printExit();
void printCredit(); 
void printMenu();