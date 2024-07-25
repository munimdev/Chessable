#pragma once

#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "AVL.h"
#include "Hash.h"
#include "List.h"

class OpeningNode;
class Game;

class ECO
{
  public:
    std::string EcoCode;
    LinkedList<OpeningNode*> belongsToOpening;

    ECO(std::string EcoCode = "");

    friend std::ostream &operator<<(std::ostream &output, const ECO &EcoObject);

    bool operator==(ECO &EcoObject);
    bool operator!=(ECO &EcoObject);
    bool operator>(ECO &EcoObject);
    bool operator<(ECO &EcoObject);

    void addOpening(OpeningNode &opening);

    void getOpenings();
    void getOpenings(ListNode<OpeningNode*> *opening);
};

class EventLocation
{
  public:
    std::string eventLocation;

    EventLocation(std::string eventLocation = "");

    friend std::ostream &operator<<(std::ostream &output, const EventLocation &eventLocationObject);
    friend bool operator==(EventLocation &object1, EventLocation &object2);
    friend bool operator<(EventLocation &object1, EventLocation &object2);
    friend bool operator>(EventLocation &object1, EventLocation &object2);
};

class EventDate
{
  public:
    unsigned short year;
    unsigned char month;
    unsigned char day;

    EventDate(unsigned short day = 0, unsigned char month = 0, unsigned char year = 0);
    EventDate(std::string date);

    friend std::ostream &operator<<(std::ostream &output, const EventDate &eventDateObject);
    friend bool operator==(EventDate &object1, EventDate &object2);
    friend bool operator<(EventDate &object1, EventDate &object2);
    friend bool operator>(EventDate &object1, EventDate &object2);
};

class Player
{
  public:
    std::string playerName;
    int totalGames;
    int wins;
    int draws;
    int losses;

    Player(std::string playerName = "");

    friend std::ostream &operator<<(std::ostream &output, const Player &P);
    friend bool operator==(Player &object1, Player &object2);
    friend bool operator<(Player &object1, Player &object2);
    friend bool operator>(Player &object1, Player &object2);

    void mostPlayedOpening();
    void getWinrate();
};

class Event
{
  public:
    std::string eventName;
    EventLocation *eventLocation;
    EventDate *eventDate;

    Event(std::string eventName = "", EventLocation *eventLocation = nullptr, EventDate *eventDate = nullptr);

    friend std::ostream &operator<<(std::ostream &output, const Event &E);
    friend bool operator==(Event &object1, Event &object2);
    friend bool operator<(Event &object1, Event &object2);
    friend bool operator>(Event &object1, Event &object2);
};

class MoveNode
{
  public:
    std::array<std::string, 2> move;
    int moveNumber;

    MoveNode();
    MoveNode(int moveNumber, std::string whiteMove, std::string blackMove);
    MoveNode(std::string move);

    std::string &operator[](std::string &key);

    friend std::ostream &operator<<(std::ostream &output, const MoveNode &M);
    friend bool operator>(MoveNode &object1, MoveNode &object2);
    friend bool operator<(MoveNode &object1, MoveNode &object2);
};

class Game
{
  public:
    Player *whitePlayerSide;
    Player *blackPlayerSide;
    std::string result;
    int numberOfMoves;
    OpeningNode *openingPlayed;
    ECO *ECO;
    MoveNode *setofMoves;
    int whiteElo;
    int blackElo;
    Event *playedInEvent;

    Game();
    Game(Event *eventPtr, Player *whitePlayerSide, Player *blackPlayerSide, class ECO *EcoPtr, std::string result, int whiteElo, int blackElo, int totalMoves, std::string moves);

    friend bool operator<(Game &G1, Game &G2);
    friend bool operator>(Game &G1, Game &G2);
    friend bool operator==(Game &G1, Game &G2);
    friend std::ostream &operator<<(std::ostream &output, const Game &game);

    std::string getPGN() const;
    std::string getPGN(int moves) const;

    void displayTopRatedGames();
    void getGamesByNumberOfMoves(int totalMoves);
    void getMostPlayedMove(std::string PGN);
};

class OpeningNode
{
  public:
    std::string openingName;
    MoveNode *setofMoves;
    ECO *belongsToECO;
    int totalMoves;

    OpeningNode(std::string openingName = "");
    OpeningNode(std::string openingName, int totalMoves);

    friend bool operator>(OpeningNode &object1, OpeningNode &object2);
    friend bool operator<(OpeningNode &object1, OpeningNode &object2);
    friend bool operator==(OpeningNode &object1, OpeningNode &object2);

    std::string getPGN() const;

    friend std::ostream &operator<<(std::ostream &output, const OpeningNode &opening);
};
