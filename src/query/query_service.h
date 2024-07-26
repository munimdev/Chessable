#pragma once

#include <string>

#include "domain/chess_database.h"

class QueryService
{
  public:
    explicit QueryService(ChessDatabase &database);

    void getOpenings(const std::string &ecoCode);
    void printEventsAttendedByPlayer(const std::string &playerName);
    bool printPlayerWinRate(const std::string &playerName);
    void printPlayerMatchups(const std::string &playerName);
    void printGamesOfEvent(const std::string &eventName);
    void printTopPlayerOfEvent(const std::string &eventName);
    void printAllEvents();
    void prettyPrintEventTree();
    void printEventsByYear(int year);
    void printTotalGamesPlayedAtEvent(const std::string &eventName);
    void printEventsOfSameLocation(const std::string &location);
    void printPlayersOfEvent(const std::string &eventName);
    void printOpeningVariations(const std::string &openingName);
    void printMatchupScore(const std::string &player1, const std::string &player2);
    void printMatchupGames(const std::string &player1, const std::string &player2);
    void printGamesOfPlayer(const std::string &playerName);
    void printMostPlayedOpeningByPlayer(const std::string &playerName);
    void printMostPlayedMoveAfterPgn(const std::string &pgn);
    void printPlayerGamesByYear(const std::string &playerName, int year);
    void printMostPlayedOpeningByEvent(const std::string &eventName);
    void filterGamesByAverageRating(int averageRating);
    void filterGamesByMovesUpperBound(int totalMoves);

  private:
    ChessDatabase &database;
};
