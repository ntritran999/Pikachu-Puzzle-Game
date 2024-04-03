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
        while(std::getline(logo_file, line))
        {
            gotoXY(40, 10 + i);
            std::cout << R"()" << line.c_str() << R"()";
            i++;
        }

        logo_file.close();
    }
}

void printGuide()
{

}

void printLeaderBoard()
{

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

    std::cout << "Develop 1: Dinh Ngoc Anh Duong\n";
    std::cout << "Develop 2: Tran Tri Nhan\n";
    
    std::cout << "\n[PRESS ESC KEY TO GO BACK...]\n";
    int option = getch();
    while (option != ESC)
        option = getch();
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

        std::cout << "Press a number to continue.\n";
        std::cout << "1.Play\n";
        std::cout << "2.Quick Guide\n";
        std::cout << "3.Leaderboard\n";
        std::cout << "4.Quit\n";
        std::cout << "5.Credit\n";

        menu_option = getch();

        if (menu_option == '1')
        {
            system("cls");
            std::cout << "Starting game...\n";
            Sleep(1000); // delay 1 second
            startGame();
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
            std::cout << "Please choose the correct option.\n";
            Sleep(1000); // delay 1 second
        }
    }
}