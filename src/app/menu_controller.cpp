#include "app/menu_controller.h"

#include <iostream>
#include <string>

namespace
{
    std::string readPromptedLine(const char *prompt)
    {
        std::cout << prompt;
        std::cin.ignore();
        std::string value;
        std::getline(std::cin, value);
        return value;
    }
}

namespace app
{
    bool processMenuChoice(int choice, QueryService &queryService)
    {
        std::string userInput;
        switch (choice)
        {
            case -1:
                return false;
            case 1:
            {
                std::cout << "\x1b[46mEnter an ECO Code:\x1b[0m ";
                std::cin >> userInput;
                queryService.getOpenings(userInput);
                break;
            }
            case 2:
            {
                userInput = readPromptedLine("\x1b[46mEnter Player Name:\x1b[0m ");
                queryService.printEventsAttendedByPlayer(userInput);
                break;
            }
            case 3:
            {
                userInput = readPromptedLine("\x1b[46mEnter Player Name:\x1b[0m ");
                if (queryService.printPlayerWinRate(userInput))
                {
                    std::cout << std::endl;
                }
                else
                {
                    std::cout << "\x1b[0;31mThat player does not exist in our database!\x1b[0m" << std::endl;
                }
                break;
            }
            case 4:
            {
                userInput = readPromptedLine("\x1b[46mEnter Player Name:\x1b[0m ");
                queryService.printPlayerMatchups(userInput);
                std::cout << std::endl;
                break;
            }
            case 5:
            {
                userInput = readPromptedLine("\x1b[46mEnter Event Name:\x1b[0m ");
                std::cout << std::endl;
                queryService.printGamesOfEvent(userInput);
                std::cout << std::endl;
                break;
            }
            case 6:
            {
                userInput = readPromptedLine("\x1b[46mEnter Event Name:\x1b[0m ");
                std::cout << std::endl;
                queryService.printTopPlayerOfEvent(userInput);
                std::cout << std::endl;
                break;
            }
            case 7:
            {
                std::cout << "\x1b[0;32mFollowing tournaments are available in our database:\x1b[0m " << std::endl
                          << std::endl;
                queryService.printAllEvents();
                break;
            }
            case 8:
            {
                queryService.prettyPrintEventTree();
                break;
            }
            case 9:
            {
                std::cout << "\x1b[46mEnter year:\x1b[0m ";
                std::cin >> userInput;
                std::cout << std::endl;
                queryService.printEventsByYear(std::stoi(userInput));
                break;
            }
            case 10:
            {
                userInput = readPromptedLine("\x1b[46mEnter Event Name:\x1b[0m ");
                queryService.printTotalGamesPlayedAtEvent(userInput);
                std::cout << std::endl;
                break;
            }
            case 11:
            {
                userInput = readPromptedLine("\x1b[46mEnter Location:\x1b[0m ");
                std::cout << std::endl;
                queryService.printEventsOfSameLocation(userInput);
                break;
            }
            case 12:
            {
                userInput = readPromptedLine("\x1b[46mEnter Event Name:\x1b[0m ");
                std::cout << std::endl;
                queryService.printPlayersOfEvent(userInput);
                break;
            }
            case 13:
            {
                userInput = readPromptedLine("\x1b[46mEnter an opening name:\x1b[0m ");
                std::cout << std::endl;
                queryService.printOpeningVariations(userInput);
                break;
            }
            case 14:
            {
                const std::string player1 = readPromptedLine("\x1b[46mEnter player 1's name:\x1b[0m ");
                std::cout << "\x1b[46mEnter player 2's name:\x1b[0m ";
                std::string player2;
                std::getline(std::cin, player2);
                std::cout << std::endl;
                queryService.printMatchupScore(player1, player2);
                break;
            }
            case 15:
            {
                const std::string player1 = readPromptedLine("\x1b[46mEnter player 1's name:\x1b[0m ");
                std::cout << "\x1b[46mEnter player 2's name:\x1b[0m ";
                std::string player2;
                std::getline(std::cin, player2);
                std::cout << std::endl;
                queryService.printMatchupGames(player1, player2);
                break;
            }
            case 16:
            {
                userInput = readPromptedLine("\x1b[46mEnter player's name:\x1b[0m ");
                queryService.printGamesOfPlayer(userInput);
                break;
            }
            case 17:
            {
                userInput = readPromptedLine("\x1b[46mEnter player's name:\x1b[0m ");
                queryService.printMostPlayedOpeningByPlayer(userInput);
                break;
            }
            case 18:
            {
                userInput = readPromptedLine("\x1b[46mEnter PGN:\x1b[0m ");
                queryService.printMostPlayedMoveAfterPgn(userInput);
                break;
            }
            case 19:
            {
                const std::string playerName = readPromptedLine("\x1b[46mEnter player's name:\x1b[0m ");
                std::cout << "\x1b[46mEnter year:\x1b[0m ";
                int year = 0;
                std::cin >> year;
                std::cout << std::endl;
                queryService.printPlayerGamesByYear(playerName, year);
                break;
            }
            case 20:
            {
                userInput = readPromptedLine("\x1b[46mEnter event name:\x1b[0m ");
                queryService.printMostPlayedOpeningByEvent(userInput);
                break;
            }
            case 21:
            {
                std::cout << "\x1b[46mEnter lower bound for average rating:\x1b[0m ";
                std::cin >> userInput;
                std::cout << std::endl;
                queryService.filterGamesByAverageRating(std::stoi(userInput));
                break;
            }
            case 22:
            {
                std::cout << "\x1b[46mEnter upper bound for total moves:\x1b[0m ";
                std::cin >> userInput;
                std::cout << std::endl;
                queryService.filterGamesByMovesUpperBound(std::stoi(userInput));
                break;
            }
            default:
                break;
        }
        return true;
    }
}
