#pragma once

#include <algorithm>
#include <new>
#include <iostream>

#include "block.h"
#include "block_logic.h"
#include "game_logic.h"
#include "window_handler.h"

struct Node
{
    Block block;
    Node *next;
};

struct List
{
    Node *head;
    Node *tail;
};

struct GameInfo;

// LinkedList related functions
Node *createNode(Block block);
List createList(Node *p);
void addTail(List &l, Block block);
void removeHead(List &l);
void removeTail(List &l);

void initBoard(List &board, int size);
void cleanBoard(List &board);
void removeBlock(List &board, int x, int y);

Node *gotoNode(Node *head, int x, int y);
bool check_I_Node(Node *head, int x1, int y1, int x2, int y2);
bool check_L_Node(Node *head, int x1, int y1, int x2, int y2);
bool check_Z_Node(Node *head, int x1, int y1, int x2, int y2);
bool check_U_Node(Node *head, int x1, int y1, int x2, int y2, int width, int height);

void drawBoard(List board);

// Stage difficulty increase game logic
void loadChallengeMode(GameInfo &game, int duration);
void slidingLeft(List &board, int width, int height);

int findHeight(Node *head);
int findWidth(Node *head);
void resetOutofBound(int &x, int &y, int width, int height);

bool scoreIMatch(int &score, List board, int x1, int y1, int x2, int y2);
bool scoreLMatch(int &score, List board, int x1, int y1, int x2, int y2);
bool scoreZMatch(int &score, List board, int x1, int y1, int x2, int y2);
bool scoreUMatch(int &score, List board, int x1, int y1, int x2, int y2, int width, int height);

bool checkEmptyBoard(List board);
bool checkRemainPairs(List board, int width, int height);

void moveSuggestion(List board, int width, int height, Block &found1, Block &found2);