#include "query/chess_queries.h"

#include <iostream>

#include "common/pgn_utils.h"

void getOpenings(std::string EcoCode, AVLTree<ECO> &ecoTree)
{
  AVLNode<ECO> *ecoNode = ecoTree.find(ECO(EcoCode));
  if(ecoNode)
  {
    ecoNode->data.getOpenings();
  }
  else
    std::cout << "\x1b[0;31mThat ECO code does not exist!\x1b[0m" << std::endl;
}

void getPlayerMatchups(std::string playerName, HashTable<Player*> &playerTable)
{
  int playerIndex = playerTable.getIndex(playerName);
  if(playerIndex != -1)
  {
    std::cout << playerName << " has played against the following players: " << std::endl << std::endl;
    auto itr = playerTable.hashTable[playerIndex].list.begin();

    for(; itr != playerTable.hashTable[playerIndex].list.end(); itr++)
    {
      std::cout << *itr.currentNode->data << " - ";
    }
  }
  else
    std::cout << "\x1b[0;31mThat player does not exist in our database!\x1b[0m" << std::endl;
}

void getTotalGamesPlayedAtEvent(std::string eventName, HashTable<Game*> &gameTable)
{
  int eventIndex = gameTable.getIndex(eventName);
  if(eventIndex != -1)
    std::cout << "Total games played at " << eventName << " were: " << gameTable.hashTable[eventIndex].list.size;
  else
    std::cout << "\x1b[0;31mThat event does not exist in our database!\x1b[0m" << std::endl;
}

void getEventsOfSameLocation(const AVLTree<Event> &eventTree, const std::string &location)
{
  eventTree.forEachInOrder([&location](const Event &event) {
    if(event.eventLocation->eventLocation == location)
      std::cout << event << std::endl;
  });
}

void getEventsByYear(const AVLTree<Event> &eventTree, int year)
{
  eventTree.forEachInOrder([year](const Event &event) {
    if(event.eventDate->year == year)
      std::cout << event << std::endl;
  });
}

void printAllEvents(const AVLTree<Event> &eventTree)
{
  eventTree.forEachInOrder([](const Event &event) {
    std::cout << event << std::endl;
  });
}

void printPlayerData(std::string eventName, HashTable<Player*> &eventPlayersTable)
{
  int eventIndex = eventPlayersTable.getIndex(eventName);
  if(eventIndex != -1)
  {
    std::cout << "Players that played in " << eventName << " were: " << std::endl;
    auto itr = eventPlayersTable.hashTable[eventIndex].list.begin();
    for(; itr != eventPlayersTable.hashTable[eventIndex].list.end(); itr++)
    {
      std::cout << itr.currentNode->data->playerName << " - ";
    }
  }
  else
    std::cout << "\x1b[0;31mThat event does not exist in our database!\x1b[0m" << std::endl;
}

void printEventData(std::string playerName, HashTable<Event*> &playerEventTable)
{
  int playerIndex = playerEventTable.getIndex(playerName);
  if(playerIndex != -1)
  {
    std::cout << "Events attended by " << playerName << " are: " << std::endl << std::endl;
    auto itr = playerEventTable.hashTable[playerIndex].list.begin();
    for(; itr != playerEventTable.hashTable[playerIndex].list.end(); itr++)
    {
      std::cout << *itr.currentNode->data << std::endl;
    }
  }
  else
    std::cout << "\x1b[0;31mThat player does not exist in our database!\x1b[0m" << std::endl;
}

void filterGamesByRating(int averageRating, const AVLTree<Game> &gameTree)
{
  gameTree.forEachInOrder([averageRating](const Game &game) {
    if(averageRating < ((game.whiteElo + game.blackElo) / 2))
      std::cout << game << std::endl;
  });
}

void filterGamesByMoves(int totalMoves, const AVLTree<Game> &gameTree)
{
  gameTree.forEachInOrder([totalMoves](const Game &game) {
    if(totalMoves >= game.numberOfMoves)
      std::cout << game << std::endl;
  });
}

void getPlayerGamesByYear(std::string playerName, int year, HashTable<Game*> &playerGames)
{
  int index = playerGames.getIndex(playerName);
  if(index != -1)
  {
    std::cout << playerName << " has the following games in the year " << year << std::endl << std::endl;

    for(auto itr = playerGames.hashTable[index].list.begin(); itr != playerGames.hashTable[index].list.end(); itr++)
    {
      if(itr.currentNode->data->playedInEvent->eventDate->year == year)
      {
        std::cout << *itr.currentNode->data << std::endl;
      }
    }
  }
  else
    std::cout << "\x1b[0;31mThat player does not exist in our database!\x1b[0m" << std::endl;
}

void getTopPlayerOfEvent(std::string eventName, HashTable<Game*> &eventGames)
{
  Player *tempPlayerNode;
  int highestELO = 0;

  int eventIndex = eventGames.getIndex(eventName);
  if(eventIndex != -1)
  {
    for(auto iterator = eventGames.hashTable[eventIndex].list.begin(); iterator != eventGames.hashTable[eventIndex].list.end(); iterator++)
    {
      if(iterator.currentNode->data->whiteElo > highestELO)
      {
        highestELO = iterator.currentNode->data->whiteElo;
        tempPlayerNode = iterator.currentNode->data->whitePlayerSide;
      }
      if(iterator.currentNode->data->blackElo > highestELO)
      {
        highestELO = iterator.currentNode->data->blackElo;
        tempPlayerNode = iterator.currentNode->data->blackPlayerSide;
      }
    }
    std::cout << "The highest rated player of " << eventName << " is " << *tempPlayerNode << " with a rating of " << highestELO << ".";
  }
  else
    std::cout << "\x1b[0;31mThat Event does not exits in our database!\x1b[0m" << std::endl;
}

void getGamesOfEvent(std::string eventName, HashTable<Game*> &gameTable)
{
  int eventIndex = gameTable.getIndex(eventName);
  if(eventIndex != -1)
  {
    std::cout << "Games played at " << eventName << " are: " << std::endl;
    auto itr = gameTable.hashTable[eventIndex].list.begin();
    for(; itr != gameTable.hashTable[eventIndex].list.end(); itr++)
    {
      std::cout << *itr.currentNode->data << std::endl;
    }
  }
  else
    std::cout << "\x1b[0;31mThat event does not exist in our database!\x1b[0m" << std::endl;
}

void getVariationsOfOpenings(const std::string &openingName, const AVLTree<OpeningNode> &openingsTree)
{
  openingsTree.forEachInOrder([&openingName](const OpeningNode &opening) {
    if(opening.openingName.substr(0, opening.openingName.find(":")) == openingName)
      std::cout << opening << std::endl;
  });
}

void getMatchupScore(std::string player1, std::string player2, HashTable<Game*> &playerGames)
{
  int player1_GameIndex = playerGames.getIndex(player1);
  int player2_GameIndex = playerGames.getIndex(player2);

  if(player1_GameIndex != 1 && player2_GameIndex != -1)
  {
    double player1_score = 0;
    double player2_score = 0;

    for(auto game_itr = playerGames.hashTable[player1_GameIndex].list.begin(); game_itr != playerGames.hashTable[player1_GameIndex].list.end(); game_itr++)
    {
      if((game_itr.currentNode->data->whitePlayerSide->playerName == player1 || game_itr.currentNode->data->blackPlayerSide->playerName == player1) && (game_itr.currentNode->data->blackPlayerSide->playerName == player2 || game_itr.currentNode->data->whitePlayerSide->playerName == player2))
      {
        if(game_itr.currentNode->data->whitePlayerSide->playerName == player1)
        {
          if(game_itr.currentNode->data->result == "1-0")
            player1_score += 1;
          else if(game_itr.currentNode->data->result == "0-1")
            player2_score += 1;
          else
          {
            player1_score += 0.5;
            player2_score += 0.5;
          }
        }
        else
        {
          if(game_itr.currentNode->data->result == "1-0")
            player2_score += 1;
          else if(game_itr.currentNode->data->result == "0-1")
            player1_score += 1;
          else
          {
            player1_score += 0.5;
            player2_score += 0.5;
          }
        }
      }
    }
    std::cout << player1 << " " << player1_score << "-" << player2_score << " " << player2 << std::endl;
  }
  else
    std::cout << "\x1b[0;31mEither of those player does not exits in our database!\x1b[0m" << std::endl;
}

void getMatchupGames(std::string player1, std::string player2, HashTable<Game*> &playerGames)
{
  int player1_GameIndex = playerGames.getIndex(player1);
  int player2_GameIndex = playerGames.getIndex(player2);

  if(player1_GameIndex != 1 && player2_GameIndex != -1)
  {
    for(auto game_itr = playerGames.hashTable[player1_GameIndex].list.begin(); game_itr != playerGames.hashTable[player1_GameIndex].list.end(); game_itr++)
    {
      if((game_itr.currentNode->data->blackPlayerSide->playerName == player2 || game_itr.currentNode->data->whitePlayerSide->playerName == player2))
      {
        std::cout << *game_itr.currentNode->data << std::endl << std::endl;
      }
    }
  }
  else
    std::cout << "\x1b[0;31mThat player does not exits\x1b[0m" << std::endl;
}

void printGameData(std::string player1, HashTable<Game*> &playerGames)
{
  int index = playerGames.getIndex(player1);
  if(index != -1)
  {
    for(auto itr = playerGames.hashTable[index].list.begin(); itr != playerGames.hashTable[index].list.end(); itr++)
    {
      std::cout << *itr.currentNode->data << std::endl;
    }
  }
  else
    std::cout << "\x1b[0;31mThat player does not exist in our database!\x1b[0m" << std::endl;
}

void getMostPlayedOpening(std::string userInput, HashTable<Game*> &games)
{
  int index = games.getIndex(userInput);
  if(index != -1)
  {
    class OpeningInfo
    {
      public:
        ECO *ecoPtr;
        int occurence;

        OpeningInfo(ECO *ecoPtr = nullptr)
        {
          this->ecoPtr = ecoPtr;
          this->occurence = 0;
        }

        bool operator==(OpeningInfo &moveInfo)
        {
          return this->ecoPtr == moveInfo.ecoPtr;
        }
    };

    LinkedList<OpeningInfo> openingFrequency;

    for(auto itr = games.hashTable[index].list.begin(); itr != games.hashTable[index].list.end(); itr++)
    {
      openingFrequency.insert(OpeningInfo(itr.currentNode->data->ECO));
      openingFrequency.search(OpeningInfo(itr.currentNode->data->ECO));
      openingFrequency.loc->data.occurence++;
    }

    auto highestOccurence = openingFrequency.root;

    for(auto itr = openingFrequency.begin(); itr != openingFrequency.end(); itr++)
    {
      if(itr.currentNode->data.occurence > highestOccurence->data.occurence)
        highestOccurence = itr.currentNode;
    }

    std::cout << std::endl << "Most played opening of " << userInput << " is: " << std::endl;
    std::cout << "ECO Code: " << highestOccurence->data.ecoPtr->EcoCode << std::endl;
    std::cout << "Opening Name: " << highestOccurence->data.ecoPtr->belongsToOpening.root->data->openingName.substr(0, highestOccurence->data.ecoPtr->belongsToOpening.root->data->openingName.find(":")) << std::endl;
    std::cout << "Times Played: " << highestOccurence->data.occurence << std::endl;
  }
}

void copyMatchingMove(const std::string &pgn, int totalMoves, bool blackMoved, AVLTree<Game> &gameTree, LinkedList<Game*> &matchingMoveList)
{
  gameTree.forEachInOrder([&](Game &game) {
    if(game.numberOfMoves < totalMoves)
      return;
    std::string gamePGN = game.getPGN(totalMoves);
    gamePGN = gamePGN.substr(0, gamePGN.find_last_of(" "));

    if(!blackMoved)
      gamePGN = gamePGN.substr(0, gamePGN.find_last_of(" "));
    if(pgn == gamePGN)
      matchingMoveList.insert(&game);
  });
}

void getMostPlayedMove(std::string pgn, AVLTree<Game> &gameTree)
{
  int totalMoves = extractMaximum(pgn);
  LinkedList<Game*> matchingMoveList;

  class MoveInfo
  {
    public:
      std::string move;
      int occurence;

      MoveInfo()
      {
        this->occurence = 0;
      }

      MoveInfo(std::string move)
      {
        this->move = move;
        this->occurence = 0;
      }

      bool operator==(MoveInfo &moveInfo)
      {
        return this->move == moveInfo.move;
      }
  };

  LinkedList<MoveInfo> moveFrequency;

  bool blackMoved;
  std::string moveNum = std::to_string(totalMoves) + ".";
  std::string tempPGN = pgn.substr(pgn.find(moveNum) + 3);
  tempPGN.erase(0, tempPGN.find_first_of(" "));
  if(tempPGN.find(" ") == std::string::npos)
    blackMoved = 0;
  else
    blackMoved = 1;

  copyMatchingMove(pgn, totalMoves, blackMoved, gameTree, matchingMoveList);

  for(auto itr = matchingMoveList.begin(); itr != matchingMoveList.end(); itr++)
  {
    if(blackMoved)
    {
      moveFrequency.insert(MoveInfo(itr.currentNode->data->setofMoves[totalMoves].move[0]));
      if(moveFrequency.search(MoveInfo(itr.currentNode->data->setofMoves[totalMoves].move[0])))
      {
        moveFrequency.loc->data.occurence++;
      }
    }
    else
    {
      moveFrequency.insert(MoveInfo(itr.currentNode->data->setofMoves[totalMoves - 1].move[1]));
      if(moveFrequency.search(MoveInfo(itr.currentNode->data->setofMoves[totalMoves - 1].move[1])))
      {
        moveFrequency.loc->data.occurence++;
      }
    }
  }

  auto highestOccurence = moveFrequency.root;

  for(auto itr = moveFrequency.begin(); itr != moveFrequency.end(); itr++)
  {
    if(itr.currentNode->data.occurence > highestOccurence->data.occurence)
      highestOccurence = itr.currentNode;
  }

  std::cout << std::endl << "Most played move after " << pgn << " is: " << std::endl;
  std::cout << "Move: " << highestOccurence->data.move << std::endl;
  std::cout << "Times played: " << highestOccurence->data.occurence << std::endl;
}
