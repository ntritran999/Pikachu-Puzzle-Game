#include "menu.h"

void printLogo()
{
    system("cls");

    std::ifstream logo_file("./images/logo.txt");
    if (!logo_file)
    {
        std::cout << "Cannot open file!\n";
        return;
    }
    else
    {
        // Print out the logo
        std::string line;
        int i = 0;
        setColor(BLACK, LIGHT_YELLOW); // Add color for the logo.
        while(std::getline(logo_file, line))
        {
            gotoXY(40, 5 + i);
            std::cout << line;
            i++;
            Sleep(100); // Add some delay for animation.
        }
        setDefaultColor();
        logo_file.close();
    }
}

void printGuide() {
    setColor(BLACK, LIGHT_GREEN);
    std::cout << "************ HOW TO PLAY ************\n";
    setColor(BLACK, LIGHT_YELLOW);
    std::cout << "  ARROW KEY = MOVING\n";
    std::cout << "  ENTER = SELECTING\n";
    std::cout << "  'U' = UNDO SELECTING\n";
    std::cout << "  'H' = HINTING\n";
    std::cout << "  'Q' = QUITING GAME\n";
    std::cout << "\n[PRESS ESC KEY TO GO BACK...]\n";

    setDefaultColor();
    while (getch() != ESC) {
        // Wait until ESC key is pressed
    }
}

void savePlayerInfo(const PlayerInfo player) {
    std::ofstream file("PlayerInfo.bin", std::ios::out | std::ios::app | std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open file for writing.\n";
        return;
    }

    // Write data of PlayerInfo into file
    file.write(reinterpret_cast<const char*>(&player), sizeof(player));

    file.close();
}

void printLeaderBoard()
{
    const int MAX_PLAYERS = 10;  // Because we need the top 10 player for the leaderboard
    std::vector <PlayerInfo> player_lists;
    PlayerInfo player;

    // Read players' information from file.
    std::ifstream file("PlayerInfo.bin", std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cout << "No records available on this device.\n";
        Sleep(500);
        return;
    }

    // Read each player's information into an array
    // Continue reading until the number of players reaches 10 or the end of file is reached
    while (player_lists.size() < MAX_PLAYERS && file.read(reinterpret_cast<char*>(&player), sizeof(player)))
        if (player.score >= 0) // Only reading the players with proper information into the list.
            player_lists.push_back(player);
    
    file.close();

    // Sort the list of players in descending order based on their scores.
    std::sort(player_lists.begin(), player_lists.end(),
            [](const PlayerInfo& player1, const PlayerInfo& player2) {
            return player1.score > player2.score;
        });

    // Print out the top 10 players or all of players if the total players is less than 10.
    setColor(BLACK, LIGHT_GREEN);
    std::cout << "========LEADERBOARD========\n";
    setColor(BLACK, LIGHT_YELLOW);
    for (int i = 0; i < player_lists.size(); ++i) {
        std::cout << "  RANK: " << i + 1 << "\n";
        std::cout << "PLAYER NAME: " << player_lists[i].playerName << std::endl;
        std::cout << "SCORE: " << player_lists[i].score << std::endl;
        std::cout << "MODE: " << player_lists[i].mode << std::endl;
        std::cout << "STATUS: " << player_lists[i].status << std::endl;
        std::cout << std::endl;
    }
    setDefaultColor();

    std::cout << "\n[PRESS ESC KEY TO GO BACK...]\n";

    while (getch() != ESC) {
        // Wait until ESC key is pressed
    }
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
    while (getch() != ESC) {
        // Wait until ESC key is pressed
    }
}

void printMenu()
{
    playSound(BACKGROUND_SOUND);
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

            // Enter the name for player 
            std::cout << "Enter your name: ";
            std::cin.getline(player.playerName, 256);

            // Start the game
            startGame(&game);

            // After finishing the game, assign information from GameInfo to PlayerInfo.
            // Assign score
            player.score = game.score;

            // Assign mode
            if (game.difficulty == EASY)
                strcpy(player.mode, "Easy");
            else if (game.difficulty == MEDIUM)
                strcpy(player.mode, "Medium");
            else if (game.difficulty == HARD)
                strcpy(player.mode, "Hard");
            else if (game.difficulty == CHALLENGE)
                strcpy(player.mode, "Challenge");
            else
                strcpy(player.mode, "Unknown");

            // Assign status
            if (game.gameFinished == true)
                strcpy(player.status, "Won");
            else
                strcpy (player.status, "Lost");

            // Save player's information into a file.
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
            playSound(ERROR_SOUND);
            std::cout << "Please choose the correct option.\n";
            Sleep(1000); // delay 1 second
        }
    }
}