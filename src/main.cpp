#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Parser.h"
#include "app/run_chessable_cli.h"
#include "cli/menu.h"
#include "domain/chess_types.h"
#include "query/chess_queries.h"

using namespace std;

int runChessableCli(const ChessableConfig &config)
{
  AVLTree<EventDate> eventDateTree; // Stores all event's Date
  AVLTree<EventLocation> eventLocationTree; // Stores all Event's location
  AVLTree<Event> eventTree; // Stores all Events

  AVLTree<Player> playerTree; // Stores all player's data
  AVLTree<Game> gameTree; // Stores all the Game data
  AVLTree<ECO> ecoTree; // Stores all ECO data
  AVLTree<OpeningNode> openingsTree; // Stores all the Opening data -> Name and Variations

  //reading the whole stuff from the big file 

  cout << endl << "Loading the chess data set." << endl;

  std::ifstream pgnfile(config.pgnPath); //open data file
  pgn::GameCollection games;
  pgnfile >> games; //parse data file

  // now counting and printing the result
	std::cout << std::endl << "The dataset contains " << games.size() << " games." << std::endl << std::endl;

  // Stores all players that played at event
  HashTable<Player *> eventPlayers( games.size()/10 ) ;
  
  // Stores all the games of event
  HashTable<Game *> eventGames( games.size()/10 );
  
  // Stores games of player
  HashTable<Game *> playerGames( 1300 );
  
  // Stores all player matchups -> player played with other players
  HashTable<Player *> playerMatchups(1300);
  
  // Stores all pointer of event that player had played 
  HashTable<Event *> playerEvents(1300);
  
  std::ifstream chessDataset; //parses the pgndata.txt file for opening data
  chessDataset.open(config.openingsPath,std::ifstream::in);
  string oname; //temp variable for storing opening name
  string EcoCode; //tmep variable for storing ECO code
  int move_number;
  char dot;
  string whiteMove, blackMove, pgn;
  chessDataset.peek(); //simply checks the file stream pointer without advancing it
  while(chessDataset.good()) //loop over each line until EOF is reached
  {
    getline(chessDataset, EcoCode, '\t'); //get ecocode
    AVLNode<ECO>* ecoNode = ecoTree.find(ECO(EcoCode));
    if(ecoNode == nullptr)
      ecoNode = ecoTree.insertAndGet(ECO(EcoCode)); //insert into ecoTree

    getline(chessDataset, oname, '\t'); //get opening name
    getline(chessDataset, pgn); //get pgn of the opening

    int totalMoves = extractMaximum(pgn); //returns the number of moves played in the opening
    AVLNode<OpeningNode>* openingNode = openingsTree.insertAndGet(OpeningNode{oname, totalMoves}); //as each opening is always unique, we need to insert it
    stringstream moveStream(pgn); //creates a string stream of opening pgn for move insertion purposes
    while(moveStream >> move_number >> dot >> whiteMove)
    {
      openingNode->data.setofMoves[move_number-1].moveNumber = move_number;
      openingNode->data.setofMoves[move_number-1].move[0] = whiteMove;
      if(moveStream >> blackMove)
        openingNode->data.setofMoves[move_number-1].move[1] = blackMove;
      else
        openingNode->data.setofMoves[move_number-1].move[1] = "";
    }
    ecoNode->data.addOpening(openingNode->data); //add the opening to the ECO node
    openingNode->data.belongsToECO = &ecoNode->data; //add a pointer of ECO to the opening that was just inserted
    chessDataset.peek(); //checks the state of the stream - whether we have reached EOF or not
  }
  chessDataset.close(); //close the file

  pgn::GameCollection::iterator itr; //define an interator for all the games in the data file
  bool makeNewEvent; //tells if new event node is created

  //temporary pointers used for insertion purposes later on
  Event* eventPtr = NULL;
  Player* whitePlayerSidePtr = NULL;
  Player* blackPlayerSidePtr = NULL;

  for (itr = games.begin(); itr != games.end(); itr++) //iterate through all the games in the file, and storing them
  {
    if(eventGames.getIndex( itr->tags()["Event"].value()) == -1 ) //search the hashTable of eventGames to see if the event exists
    //as an event cannot exist without game(s), we simply see if the current event being parsed has an empty hash index or not
    {
      makeNewEvent = true; //set makeNewEvent to true if an event does not already exist
    }
    else {
      makeNewEvent = false; //else set it to fast
    }
    
    AVLNode<EventLocation>* eventLocationNode = eventLocationTree.find(itr->tags()["Site"].value());
    if(eventLocationNode == nullptr)
      eventLocationNode = eventLocationTree.insertAndGet(itr->tags()["Site"].value());
    AVLNode<EventDate>* eventDateNode = eventDateTree.find(itr->tags()["EventDate"].value());
    if(eventDateNode == nullptr)
      eventDateNode = eventDateTree.insertAndGet(itr->tags()["EventDate"].value());
    if(makeNewEvent) //if we are creating a new event node in the eventTree
    {
      AVLNode<Event>* eventNode = eventTree.insertAndGet(Event(itr->tags()["Event"].value(), &eventLocationNode->data, &eventDateNode->data));
      eventPtr = &eventNode->data; //save the address of the newly created event
    } 
    else //if the event node already exists in the list, we will need to save its address
    {
      eventPtr = eventGames.hashTable[eventGames.getIndex(itr->tags()["Event"].value())].list.root->data->playedInEvent;
    }

    AVLNode<Player>* whitePlayerNode = playerTree.find(itr->tags()["White"].value());
    if(whitePlayerNode == nullptr)
      whitePlayerNode = playerTree.insertAndGet(itr->tags()["White"].value());
    whitePlayerSidePtr = &whitePlayerNode->data;

    AVLNode<Player>* blackPlayerNode = playerTree.find(itr->tags()["Black"].value());
    if(blackPlayerNode == nullptr)
      blackPlayerNode = playerTree.insertAndGet(itr->tags()["Black"].value());
    blackPlayerSidePtr = &blackPlayerNode->data;
  
    //update hash table pointers
    //update players of event
    eventPlayers.doubleHashInsert( eventPtr->eventName, whitePlayerSidePtr); 
    eventPlayers.doubleHashInsert( eventPtr->eventName, blackPlayerSidePtr); 
    //update event of players
    playerEvents.doubleHashInsert(whitePlayerSidePtr->playerName, eventPtr);  
    playerEvents.doubleHashInsert(blackPlayerSidePtr->playerName, eventPtr); 

    pgn::Game game = *itr; //parses the current game
    pgn::MoveList moveList = game.moves(); //stores the list of moves in moveList

    AVLNode<ECO>* ecoNode = ecoTree.find(itr->tags()["ECO"].value());
    if(ecoNode == nullptr)
      ecoNode = ecoTree.insertAndGet(itr->tags()["ECO"].value());
    
    pgn = moveList.getPGN(); //get the moves in PGN format

    Game *newGame = new Game(eventPtr, whitePlayerSidePtr, blackPlayerSidePtr, &ecoNode->data, itr->tags()["Result"].value(),  stoi(itr->tags()["WhiteElo"].value()), stoi(itr->tags()["BlackElo"].value()), moveList.size(), pgn); //create a new game object

    //update player score based on game result
    if(itr->tags()["Result"].value() == "1-0")
    {
      whitePlayerSidePtr->wins++;
      blackPlayerSidePtr->losses++;
    }
    else if(itr->tags()["Result"].value() == "0-1")
    {
      blackPlayerSidePtr->wins++;
      whitePlayerSidePtr->losses++;
    }
    else
    {
      whitePlayerSidePtr->draws++;
      blackPlayerSidePtr->draws++;
    }
    whitePlayerSidePtr->totalGames++;
    blackPlayerSidePtr->totalGames++;

    //update player matchups
    playerMatchups.doubleHashInsert(whitePlayerSidePtr->playerName, blackPlayerSidePtr);
    playerMatchups.doubleHashInsert(blackPlayerSidePtr->playerName, whitePlayerSidePtr);

    //update games of an event
    eventGames.doubleHashInsert(eventPtr->eventName, newGame);

    //update player games
    playerGames.doubleHashInsert(whitePlayerSidePtr->playerName, newGame);
    playerGames.doubleHashInsert(blackPlayerSidePtr->playerName, newGame);

    //inserts new game into the tree
    AVLNode<Game>* gameNode = gameTree.insertAndGet(*newGame);
    gameNode->data.playedInEvent = eventPtr; //stores pointer to event in the game
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
        getOpenings(opening, ecoTree);
      }; break;

      // Events Attended by player
      case 2:
      {
        cout << "\e[46mEnter Player Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        printEventData(userInput, playerEvents);
      }; break;
      
      // win Rate
      case 3:
      {
        cout << "\e[46mEnter Player Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        auto searchp = Player(userInput);
        AVLNode<Player>* playerNode = playerTree.find(searchp);
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
        getPlayerMatchups(userInput, playerMatchups);
        cout << endl;
       }; break;

      // Get games of event
      case 5:
      {
        cout << "\e[46mEnter Event Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        getGamesOfEvent(userInput, eventGames);
        cout << endl;
      }; break;

      // Get Top Player Of Event
      case 6:
      {
        cout << "\e[46mEnter Event Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        getTopPlayerOfEvent(userInput, eventGames);
        cout << endl;
      }; break;

      // Prints All events
      case 7:
      {
        cout << "\e[0;32mFollowing tournaments are available in our database:\x1b[0m " << endl << endl;
        printAllEvents(eventTree);
      }; break;

      // Pretty Print Event List
      case 8:
      {
        eventTree.prettyPrint();
      }; break;

      // Get Events By year
      case 9:
      {
        cout << "\e[46mEnter year:\x1b[0m ";
        cin >> userInput;
        cout << endl;
        getEventsByYear(eventTree, stoi(userInput));
      }; break;

      // Get Total Games Played At Event
      case 10:
      {
        cout << "\e[46mEnter Event Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getTotalGamesPlayedAtEvent(userInput, eventGames);
        cout << endl;
      }; break;

      // Get Events Of Same Location
      case 11:
      {
        cout << "\e[46mEnter Location:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        getEventsOfSameLocation(eventTree, userInput);
      }; break;

      // Players that attened certain event
      case 12:
      {
        cout << "\e[46mEnter Event Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        printPlayerData (userInput, eventPlayers);
      }; break;

      //Displays variations of an opening
      case 13:
      {
        cout << "\e[46mEnter an opening name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        getVariationsOfOpenings(userInput, openingsTree);
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
        getMatchupScore(userInput, player2, playerGames);
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
        getMatchupGames(userInput, player2, playerGames);
      }; break;

      //Gets lifetime games of a player
      case 16:
      {
        cout << "\e[46mEnter player's name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        printGameData(userInput, playerGames);
      }; break;

      //Prints the most played opening by a player
      case 17:
      {
        cout << "\e[46mEnter player's name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getMostPlayedOpening(userInput, playerGames);
      }; break;
    
      //given a chess position, the program outputs the most played move
      case 18:
      {
        cout << "\e[46mEnter PGN:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getMostPlayedMove(userInput, gameTree);
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
        getPlayerGamesByYear(userInput, year, playerGames);
      }; break;

      case 20:
      {
        cout << "\e[46mEnter event name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getMostPlayedOpening(userInput, eventGames);
      }; break;

      case 21:
      {
        cout << "\e[46mEnter lower bound for average rating:\x1b[0m ";
        cin >> userInput;
        cout << endl;
        filterGamesByRating(stoi(userInput), gameTree);
      }; break;

      case 22:
      {
        cout << "\e[46mEnter upper bound for total moves:\x1b[0m ";
        cin >> userInput;
        cout << endl;
        filterGamesByMoves(stoi(userInput), gameTree);
      }; break;
  }
  } while (choice != -1);

  return 0;
}