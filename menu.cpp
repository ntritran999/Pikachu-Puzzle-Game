#include "menu.h"

void printLogo()
{
    system("cls");

    std::ifstream logo_file(R"(.\images\logo.txt)");
    if (!logo_file.is_open())
    {
        std::cout << "Cannot open file!\n";
        return;
    }
    else
    {
        std::string line;
        int i = 0;
        setColor(BLACK, LIGHT_YELLOW);
        while(std::getline(logo_file, line))
        {
            gotoXY(40, 5 + i);
            std::cout << R"()" << line.c_str() << R"()";
            i++;
            Sleep(100);
        }
        setDefaultColor();
        logo_file.close();
    }
}

void printGuide() {
    system("cls");
    std::cout << "************ GUIDE ************\n";
    std::cout << "Pikachu Online is a classic puzzle game where you need to match pairs of identical tiles.\n";
    std::cout << "The rules are simple:\n";
    std::cout << "1. You can only match tiles that are not blocked from the left or right and can be connected with three or fewer straight lines.\n";
    std::cout << "2. If you match a pair of tiles, they will disappear, and you will earn points.\n";
    std::cout << "3. Your goal is to clear all tiles from the board before time runs out.\n";
    std::cout << "4. Be careful! The game will become more challenging as you progress to higher levels.\n";
    std::cout << "\n";
    std::cout << "************ HOW TO PLAY ************\n";
    std::cout << "Use arrow keys to move the cursor.\n";
    std::cout << "Press Enter to select a tile.\n";
    std::cout << "Press 'H' to ask for a hint.\n";
    std::cout << "Press 'Q' to quit the game.\n";
    std::cout << "\n[PRESS ESC KEY TO GO BACK...]\n";

    while (getch() != ESC) {
        // Wait until ESC key is pressed
    }
}


void printLeaderBoard()
{
    const int MAX_PLAYERS = 10;  // Do cần in thông tin từ 10 người chơi trở xuống
    PlayerInfo* players = new PlayerInfo[MAX_PLAYERS];
    PlayerInfo player;
    int numPlayers = 0;

    // Đọc thông tin người chơi từ file
    std::ifstream file("PlayerInfo.txt", std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open file for reading.\n";
        delete[] players; // Giải phóng bộ nhớ
        return;
    }

    // Đọc thông tin của từng người chơi vào mảng
    while (numPlayers < MAX_PLAYERS && file.read(reinterpret_cast<char*>(&player), sizeof(PlayerInfo))) {
        players[numPlayers++] = player;
    }
    file.close();

    // Sắp xếp mảng theo điểm số giảm dần
    for (int i = 0; i < numPlayers - 1; ++i) {
        for (int j = i + 1; j < numPlayers; ++j) {
            if (players[i].score < players[j].score) {
                std::swap(players[i], players[j]);
            }
        }
    }

    // In ra thông tin của 10 người chơi có điểm số cao nhất hoặc tất cả người chơi nếu số lượng ít hơn 10
    
    std::cout << "Leaderboard:\n";
    
    for (int i = 0; i < numPlayers; ++i) {
        std::cout << "Rank " << i + 1 << ":\n";
        std::cout << "Player Name: " << players[i].playerName << std::endl;
        std::cout << "Score: " << players[i].score << std::endl;
        std::cout << "Mode: " << players[i].mode << std::endl;
        std::cout << std::endl;
    }

    delete[] players; // Giải phóng bộ nhớ
}

bool printExit()
{
    std::cout << "Do you wish to exit?: [0 == yes / 1 == no]\n";
    int option = getch();
    
    while (option != '0' && option != '1')
    {
        std::cout << "Please type 0 or 1.\n";
        option = getch();
    }

    if (option == '0')
    {
        system("cls");
        std::cout << "Thank you for playing this game.\n";
        return true;
    }

    system("cls");
    std::cout << "Returning...\n";
       
    return false;
}

void printCredit()
{
    system("cls");

    std::cout << "DEVELOPER 1: DINH NGOC ANH DUONG\n";
    std::cout << "DEVELOPER 2: TRAN TRI NHAN\n";
    
    std::cout << "\n[PRESS ESC KEY TO GO BACK...]\n";
    int option = getch();
    while (option != ESC)
        option = getch();
}

void savePlayerInfo(const PlayerInfo& player) {
    std::ofstream file("PlayerInfo.txt", std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open file for writing.\n";
        return;
    }

    // Ghi dữ liệu của PlayerInfo vào file
    file.write(reinterpret_cast<const char*>(&player), sizeof(PlayerInfo));

    file.close();
}

void printPlayerInfo() {
    PlayerInfo player;
    std::ifstream file("PlayerInfo.txt", std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open file for reading.\n";
        return;
    }

    std::cout << "Player info:\n";
    while (file.read(reinterpret_cast<char*>(&player), sizeof(PlayerInfo))) {
        std::cout << "Player Name: " << player.playerName << std::endl;
        std::cout << "Score: " << player.score << std::endl;
        std::cout << "Mode: " << player.mode << std::endl;
        std::cout << std::endl;
    }
    file.close();
}

void printMenu()
{
    printLogo();
    Sleep(3000);
    
    system("cls");
    std::cout << "Loading menu...\n";
    Sleep(1000);

    int menu_option;

    while (true)
    {
        system("cls");

        std::cout << "-----PRESS A NUMBER TO CONTINUE-----\n";
        setColor(BLACK, LIGHT_YELLOW);
        std::cout << "  1.PLAY\n";
        std::cout << "  2.QUICK GUIDE\n";
        std::cout << "  3.LEADERBOARD\n";
        std::cout << "  4.QUIT\n";
        std::cout << "  5.CREDIT\n";
        setDefaultColor();

        menu_option = getch();

        if (menu_option == '1')
        {
            system("cls");
            std::cout << "Starting game...\n";
            Sleep(1000); // delay 1 second

             GameInfo game;
            PlayerInfo player;

            // Nhập tên người chơi
            std::cout << "Enter your name: ";
            std::getline(std::cin, player.playerName);

            startGame(&game);

            // Gán thông tin điểm và độ khó từ GameInfo vào PlayerInfo
            player.score = game.score;
            player.mode = game.difficulty;

            // Lưu thông tin người chơi vào file
            savePlayerInfo(player);
        }

        else if (menu_option == '2')
        {
            system("cls");
            printGuide();
        }

        else if (menu_option == '3')
        {
            system("cls");
            printPlayerInfo();
            printLeaderBoard();
        }

        else if (menu_option == '4')
        {
            if (printExit())
            {
                Sleep(1000);  // delay 1 second  
                break;
            }
        }

        else if (menu_option == '5')
            printCredit();

        else
        {
            std::cout << "Please choose the correct option.\n";
            Sleep(1000); // delay 1 second
        }
    }
}