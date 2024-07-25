#include <fstream>
#include <iostream>
#include <string>

#include "Parser.h"
#include "app/run_chessable_cli.h"
#include "cli/menu.h"
#include "domain/chess_database.h"
#include "ingest/database_loader.h"
#include "domain/chess_types.h"
#include "query/chess_queries.h"

using namespace std;

int runChessableCli(const ChessableConfig &config)
{
  std::ifstream pgnfile(config.pgnPath);
  pgn::GameCollection games;
  pgnfile >> games;
  ChessDatabase database(games.size());
  if(!loadChessDatabase(config, database, cout, cerr))
  {
    return 1;
  }
  
  int choice;
  string opening, userInput;
  do {
    cli::printMainMenu();
    choice = cli::readMenuChoice();
    switch(choice)  //switch statement to check user input and calling respective functions
    {
      case -1:
        break;
      
      // Get openings
      case 1:
      {
        cout << "\e[46mEnter an ECO Code:\x1b[0m "; cin >> opening;
        getOpenings(opening, database.ecoTree);
      }; break;

      // Events Attended by player
      case 2:
      {
        cout << "\e[46mEnter Player Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        printEventData(userInput, database.playerEvents);
      }; break;
      
      // win Rate
      case 3:
      {
        cout << "\e[46mEnter Player Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        auto searchp = Player(userInput);
        AVLNode<Player>* playerNode = database.playerTree.find(searchp);
        if(playerNode)
        {
          playerNode->data.getWinrate();
          cout << endl;
        }
        else
          cout << "\e[0;31mThat player does not exist in our database!\x1b[0m" << endl;
      }; break;
      
      // Get Player MatchUps
      case 4:
      {
        cout << "\e[46mEnter Player Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getPlayerMatchups(userInput, database.playerMatchups);
        cout << endl;
       }; break;

      // Get games of event
      case 5:
      {
        cout << "\e[46mEnter Event Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        getGamesOfEvent(userInput, database.eventGames);
        cout << endl;
      }; break;

      // Get Top Player Of Event
      case 6:
      {
        cout << "\e[46mEnter Event Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        getTopPlayerOfEvent(userInput, database.eventGames);
        cout << endl;
      }; break;

      // Prints All events
      case 7:
      {
        cout << "\e[0;32mFollowing tournaments are available in our database:\x1b[0m " << endl << endl;
        printAllEvents(database.eventTree);
      }; break;

      // Pretty Print Event List
      case 8:
      {
        database.eventTree.prettyPrint();
      }; break;

      // Get Events By year
      case 9:
      {
        cout << "\e[46mEnter year:\x1b[0m ";
        cin >> userInput;
        cout << endl;
        getEventsByYear(database.eventTree, stoi(userInput));
      }; break;

      // Get Total Games Played At Event
      case 10:
      {
        cout << "\e[46mEnter Event Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getTotalGamesPlayedAtEvent(userInput, database.eventGames);
        cout << endl;
      }; break;

      // Get Events Of Same Location
      case 11:
      {
        cout << "\e[46mEnter Location:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        getEventsOfSameLocation(database.eventTree, userInput);
      }; break;

      // Players that attened certain event
      case 12:
      {
        cout << "\e[46mEnter Event Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        printPlayerData(userInput, database.eventPlayers);
      }; break;

      //Displays variations of an opening
      case 13:
      {
        cout << "\e[46mEnter an opening name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        getVariationsOfOpenings(userInput, database.openingsTree);
      }; break;

      //Gets lifetime score between 2 players
      case 14:
      {
        string player2;
        cout << "\e[46mEnter player 1's name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << "\e[46mEnter player 2's name:\x1b[0m ";
        getline(cin, player2);
        cout << endl;
        getMatchupScore(userInput, player2, database.playerGames);
      }; break;
  
      //Gets lifetime games between 2 players
      case 15:
      {
        string player2;
        cout << "\e[46mEnter player 1's name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << "\e[46mEnter player 2's name:\x1b[0m ";
        getline(cin, player2);
        cout << endl;
        getMatchupGames(userInput, player2, database.playerGames);
      }; break;

      //Gets lifetime games of a player
      case 16:
      {
        cout << "\e[46mEnter player's name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        printGameData(userInput, database.playerGames);
      }; break;

      //Prints the most played opening by a player
      case 17:
      {
        cout << "\e[46mEnter player's name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getMostPlayedOpening(userInput, database.playerGames);
      }; break;
    
      //given a chess position, the program outputs the most played move
      case 18:
      {
        cout << "\e[46mEnter PGN:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getMostPlayedMove(userInput, database.gameTree);
      }; break;

      //prints all games of a player played in a certain year
      case 19:
      {
        int year;
        cout << "\e[46mEnter player's name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << "\e[46mEnter year:\x1b[0m ";
        cin >> year;
        cout << endl;
        getPlayerGamesByYear(userInput, year, database.playerGames);
      }; break;

      case 20:
      {
        cout << "\e[46mEnter event name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getMostPlayedOpening(userInput, database.eventGames);
      }; break;

      case 21:
      {
        cout << "\e[46mEnter lower bound for average rating:\x1b[0m ";
        cin >> userInput;
        cout << endl;
        filterGamesByRating(stoi(userInput), database.gameTree);
      }; break;

      case 22:
      {
        cout << "\e[46mEnter upper bound for total moves:\x1b[0m ";
        cin >> userInput;
        cout << endl;
        filterGamesByMoves(stoi(userInput), database.gameTree);
      }; break;
  }
  } while (choice != -1);

  return 0;
}