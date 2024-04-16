#include "window_handler.h"
#include <iostream>

HANDLE consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
HWND consoleWindow = GetConsoleWindow();

void gotoXY(int x, int y)
{
    SetConsoleCursorPosition(consoleOutput, COORD{ (short)x, (short)y });
}

void setUpWindow()
{
    setWindowSize();
    setTitle();
    hideCursor();
    hideScrollBar();
}

void setWindowSize()
{
    RECT desktop;

    GetWindowRect(GetDesktopWindow(), &desktop);

    MoveWindow(consoleWindow, 0, 0, desktop.right, desktop.bottom, TRUE);
}

void setColor(int background, int text)
{
    SetConsoleTextAttribute(consoleOutput, background * 16 + text);
}

void setDefaultColor()
{
    SetConsoleTextAttribute(consoleOutput, 15);
}

void hideScrollBar()
{
    ShowScrollBar(consoleWindow, SB_BOTH, 0);
}

void hideCursor()
{
    CONSOLE_CURSOR_INFO ConCurInf;
    
    ConCurInf.dwSize = 10;
    ConCurInf.bVisible = false;
    
    SetConsoleCursorInfo(consoleOutput, &ConCurInf);
}

void setTitle()
{
    SetConsoleTitleA("PIKACHU-PUZZLE-GAME");
}

// Sound setting
void playSound(int audio_pos) {
    std::string soundFile[] = { "background.wav", "move.wav", "lock.wav", "unlock.wav", "error.wav", "endgame.wav" };
    // Play audio from the declared path.
    std::string path = "./sounds/" + soundFile[audio_pos];
    PlaySound(path.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}