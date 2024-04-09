#pragma once

#include "window_handler.h"
#include "game_logic.h"


struct PlayerInfo {
	std::string playerName;
	int score;
	std::string mode;
};

void printLogo();
void printGuide();
void printLeaderBoard();
bool printExit();
void printCredit(); 
void printMenu();