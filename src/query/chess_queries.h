#pragma once

#include <string>

#include "domain/chess_types.h"

void getOpenings(std::string EcoCode, AVLTree<ECO> &ecoTree);
void getPlayerMatchups(std::string playerName, HashTable<Player*> &playerTable);
void getTotalGamesPlayedAtEvent(std::string eventName, HashTable<Game*> &gameTable);
void getEventsOfSameLocation(const AVLTree<Event> &eventTree, const std::string &location);
void getEventsByYear(const AVLTree<Event> &eventTree, int year);
void printAllEvents(const AVLTree<Event> &eventTree);
void printPlayerData(std::string eventName, HashTable<Player*> &eventPlayersTable);
void printEventData(std::string playerName, HashTable<Event*> &playerEventTable);
void filterGamesByRating(int averageRating, const AVLTree<Game> &gameTree);
void filterGamesByMoves(int totalMoves, const AVLTree<Game> &gameTree);
void getPlayerGamesByYear(std::string playerName, int year, HashTable<Game*> &playerGames);
void getTopPlayerOfEvent(std::string eventName, HashTable<Game*> &eventGames);
void getGamesOfEvent(std::string eventName, HashTable<Game*> &gameTable);
void getVariationsOfOpenings(const std::string &openingName, const AVLTree<OpeningNode> &openingsTree);
void getMatchupScore(std::string player1, std::string player2, HashTable<Game*> &playerGames);
void getMatchupGames(std::string player1, std::string player2, HashTable<Game*> &playerGames);
void printGameData(std::string player1, HashTable<Game*> &playerGames);
void getMostPlayedOpening(std::string userInput, HashTable<Game*> &games);
void copyMatchingMove(const std::string &pgn, int totalMoves, bool blackMoved, AVLTree<Game> &gameTree, LinkedList<Game*> &matchingMoveList);
void getMostPlayedMove(std::string pgn, AVLTree<Game> &gameTree);
