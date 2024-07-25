#include "domain/chess_types.h"

#include <algorithm>

ECO::ECO(std::string EcoCode)
{
  this->EcoCode = EcoCode;
}

std::ostream &operator<<(std::ostream &output, const ECO &EcoObject)
{
  output << EcoObject.EcoCode;
  return output;
}

bool ECO::operator==(ECO &EcoObject)
{
  return this->EcoCode == EcoObject.EcoCode;
}

bool ECO::operator!=(ECO &EcoObject)
{
  return this->EcoCode != EcoObject.EcoCode;
}

bool ECO::operator>(ECO &EcoObject)
{
  return this->EcoCode > EcoObject.EcoCode;
}

bool ECO::operator<(ECO &EcoObject)
{
  return this->EcoCode < EcoObject.EcoCode;
}

void ECO::addOpening(OpeningNode &opening)
{
  this->belongsToOpening.insert(&opening);
}

void ECO::getOpenings()
{
  std::cout << this->EcoCode << " contains the following openings: " << std::endl;
  getOpenings(belongsToOpening.root);
}

void ECO::getOpenings(ListNode<OpeningNode*> *opening)
{
  if(opening)
  {
    std::cout << *opening->data << std::endl;
    getOpenings(opening->next);
  }
  if(opening == this->belongsToOpening.root)
    std::cout << std::endl;
}

EventLocation::EventLocation(std::string eventLocation)
{
  this->eventLocation = eventLocation;
}

std::ostream &operator<<(std::ostream &output, const EventLocation &eventLocationObject)
{
  output << eventLocationObject.eventLocation;
  return output;
}

bool operator==(EventLocation &object1, EventLocation &object2)
{
  return object1.eventLocation == object2.eventLocation;
}

bool operator<(EventLocation &object1, EventLocation &object2)
{
  return object1.eventLocation < object2.eventLocation;
}

bool operator>(EventLocation &object1, EventLocation &object2)
{
  return object1.eventLocation > object2.eventLocation;
}

EventDate::EventDate(unsigned short day, unsigned char month, unsigned char year)
{
  this->day = day;
  this->month = month;
  this->year = year;
}

EventDate::EventDate(std::string date)
{
  if(date.substr(0, date.find(".")) != "????")
    this->year = static_cast<unsigned short>(std::stoi(date.substr(0, date.find("."))));
  else
    this->year = 0;
  date.erase(0, date.find_first_of(".") + 1);
  if(date.substr(0, date.find(".")) != "??")
    this->month = static_cast<unsigned char>(std::stoi(date.substr(0, date.find("."))));
  else
    this->month = 0;
  date.erase(0, date.find_first_of(".") + 1);
  if(date.substr(0, date.find(".")) != "??")
    this->day = static_cast<unsigned char>(std::stoi(date.substr(0, date.find("."))));
  else
    this->day = 0;
}

std::ostream &operator<<(std::ostream &output, const EventDate &eventDateObject)
{
  output << eventDateObject.year << '.' << int(eventDateObject.month) << '.' << int(eventDateObject.day);
  return output;
}

bool operator==(EventDate &object1, EventDate &object2)
{
  return object1.day == object2.day && object1.month == object2.month && object1.year == object2.year;
}

bool operator<(EventDate &object1, EventDate &object2)
{
  if(object1.year < object2.year)
    return true;
  else if(object1.year == object2.year)
  {
    if(object1.month < object2.month)
      return true;
    else if(object1.month == object2.month)
      return object1.day < object2.day;
    else
      return false;
  }
  else
    return false;
}

bool operator>(EventDate &object1, EventDate &object2)
{
  if(object1.year > object2.year)
    return true;
  else if(object1.year == object2.year)
  {
    if(object1.month > object2.month)
      return true;
    else if(object1.month == object2.month)
      return object1.day > object2.day;
    else
      return false;
  }
  else
    return false;
}

Player::Player(std::string playerName)
{
  this->playerName = playerName;
  this->wins = 0;
  this->draws = 0;
  this->losses = 0;
  this->totalGames = 0;
}

std::ostream &operator<<(std::ostream &output, const Player &P)
{
  output << P.playerName;
  return output;
}

void Player::getWinrate()
{
  std::cout << "Win rate of " << playerName << " is: " << 100 * double(wins + double(draws / 2)) / (totalGames) << "%";
}

bool operator==(Player &object1, Player &object2)
{
  return object1.playerName == object2.playerName;
}

bool operator<(Player &object1, Player &object2)
{
  return object1.playerName < object2.playerName;
}

bool operator>(Player &object1, Player &object2)
{
  return object1.playerName > object2.playerName;
}

Event::Event(std::string eventName, EventLocation *eventLocation, EventDate *eventDate)
{
  this->eventName = eventName;
  this->eventLocation = eventLocation;
  this->eventDate = eventDate;
}

std::ostream &operator<<(std::ostream &output, const Event &E)
{
  output << "[Event " << "\"" << E.eventName << "\"]" << std::endl;
  output << "[Site " << "\"" << *E.eventLocation << "\"]" << std::endl;
  output << "[Date " << "\"" << *E.eventDate << "\"]" << std::endl;
  return output;
}

bool operator==(Event &object1, Event &object2)
{
  return object1.eventName == object2.eventName;
}

bool operator<(Event &object1, Event &object2)
{
  return object1.eventName < object2.eventName;
}

bool operator>(Event &object1, Event &object2)
{
  return object1.eventName > object2.eventName;
}

MoveNode::MoveNode()
{
  this->move.fill(std::string("\0", 1));
}

MoveNode::MoveNode(int moveNumber, std::string whiteMove, std::string blackMove)
{
  this->move[0] = whiteMove;
  this->move[1] = blackMove;
  this->moveNumber = moveNumber;
}

MoveNode::MoveNode(std::string move)
{
  this->moveNumber = std::stoi(move.substr(0, move.find(".")));
  this->move[0] = move.substr(move.find_first_of(" ") + 1, move.find_last_of(" ") - move.find_first_of(" ") - 1);
  this->move[1] = move.substr(move.find_last_of(" ") + 1);
}

std::string &MoveNode::operator[](std::string &key)
{
  if(key == "White")
    return move[0];
  if(key == "Black")
    return move[1];

  throw std::runtime_error("Invalid key inputted.");
}

std::ostream &operator<<(std::ostream &output, const MoveNode &M)
{
  output << M.moveNumber << ". " << M.move[0] << " " << M.move[1] << " ";
  return output;
}

bool operator<(MoveNode &object1, MoveNode &object2)
{
  return object1.moveNumber < object2.moveNumber;
}

bool operator>(MoveNode &object1, MoveNode &object2)
{
  return object1.moveNumber > object2.moveNumber;
}

Game::Game()
{
  whitePlayerSide = nullptr;
  blackPlayerSide = nullptr;
  result = "";
  whiteElo = 0;
  blackElo = 0;
}

Game::Game(Event *eventPtr, Player *whitePlayerSide, Player *blackPlayerSide, class ECO *EcoPtr, std::string result, int whiteElo, int blackElo, int totalMoves, std::string moves)
{
  this->playedInEvent = eventPtr;
  this->whitePlayerSide = whitePlayerSide;
  this->blackPlayerSide = blackPlayerSide;
  this->ECO = EcoPtr;
  this->result = result;
  this->whiteElo = whiteElo;
  this->blackElo = blackElo;
  this->numberOfMoves = totalMoves;
  this->setofMoves = new MoveNode[numberOfMoves];

  int move_number;
  char dot;
  std::string whiteMove;
  std::string blackMove;
  std::stringstream moveStream(moves);

  while(moveStream >> move_number >> dot >> whiteMove)
  {
    setofMoves[move_number - 1].moveNumber = move_number;
    setofMoves[move_number - 1].move[0] = whiteMove;
    if(moveStream >> blackMove)
      setofMoves[move_number - 1].move[1] = blackMove;
    else
      setofMoves[move_number - 1].move[1] = std::string("\0", 1);
  }
}

bool operator<(Game &G1, Game &G2)
{
  return G1.whitePlayerSide->playerName + G1.blackPlayerSide->playerName < G2.whitePlayerSide->playerName + G2.blackPlayerSide->playerName;
}

bool operator>(Game &G1, Game &G2)
{
  return G1.whitePlayerSide->playerName + G1.blackPlayerSide->playerName > G2.whitePlayerSide->playerName + G2.blackPlayerSide->playerName;
}

bool operator==(Game &G1, Game &G2)
{
  return G1.result == G2.result && G1.whiteElo == G2.whiteElo && G1.blackElo == G2.blackElo && G1.numberOfMoves == G2.numberOfMoves && G1.whitePlayerSide->playerName == G2.whitePlayerSide->playerName && G1.blackPlayerSide->playerName == G2.blackPlayerSide->playerName;
}

std::ostream &operator<<(std::ostream &output, const Game &game)
{
  output << *game.playedInEvent;
  output << "[White "" << game.whitePlayerSide->playerName << ""]" << std::endl;
  output << "[Black "" << game.blackPlayerSide->playerName << ""]" << std::endl;
  output << "[Result "" << game.result << ""]" << std::endl;
  output << "[ECO "" << game.ECO->EcoCode << ""]" << std::endl;
  output << "[WhiteElo "" << game.whiteElo << ""]" << std::endl;
  output << "[BlackElo "" << game.blackElo << ""]" << std::endl << std::endl;
  output << game.getPGN() << std::endl;
  return output;
}

std::string Game::getPGN() const
{
  std::stringstream PGN;
  for(int i = 0; i < this->numberOfMoves; i++)
  {
    PGN << this->setofMoves[i];
  }
  return PGN.str();
}

std::string Game::getPGN(int moves) const
{
  std::stringstream PGN;
  for(int i = 0; i < moves; i++)
  {
    PGN << this->setofMoves[i];
  }
  return PGN.str();
}

OpeningNode::OpeningNode(std::string openingName)
{
  this->openingName = openingName;
}

OpeningNode::OpeningNode(std::string openingName, int totalMoves)
{
  this->openingName = openingName;
  this->totalMoves = totalMoves;
  setofMoves = new MoveNode[totalMoves];
}

std::string OpeningNode::getPGN() const
{
  std::stringstream PGN;
  for(int i = 0; i < this->totalMoves; i++)
  {
    PGN << this->setofMoves[i];
  }
  return PGN.str();
}

std::ostream &operator<<(std::ostream &output, const OpeningNode &opening)
{
  output << "\x1b[0;34mOpening name\x1b[0m :: " << opening.openingName << std::endl << "\x1b[0;34mPGN\x1b[0m :: ";
  output << opening.getPGN();
  return output;
}

bool operator<(OpeningNode &object1, OpeningNode &object2)
{
  return object1.openingName < object2.openingName;
}

bool operator>(OpeningNode &object1, OpeningNode &object2)
{
  return object1.openingName > object2.openingName;
}

bool operator==(OpeningNode &object1, OpeningNode &object2)
{
  return object1.openingName == object2.openingName;
}
