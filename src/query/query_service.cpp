#include "query/query_service.h"

#include "query/chess_queries.h"

QueryService::QueryService(ChessDatabase &database)
    : database(database)
{
}

void QueryService::getOpenings(const std::string &ecoCode)
{
    ::getOpenings(ecoCode, database.ecoTree);
}

void QueryService::printEventsAttendedByPlayer(const std::string &playerName)
{
    ::printEventData(playerName, database.playerEvents);
}

bool QueryService::printPlayerWinRate(const std::string &playerName)
{
    AVLNode<Player> *playerNode = database.playerTree.find(Player(playerName));
    if (playerNode == nullptr)
    {
        return false;
    }
    playerNode->data.getWinrate();
    return true;
}

void QueryService::printPlayerMatchups(const std::string &playerName)
{
    ::getPlayerMatchups(playerName, database.playerMatchups);
}

void QueryService::printGamesOfEvent(const std::string &eventName)
{
    ::getGamesOfEvent(eventName, database.eventGames);
}

void QueryService::printTopPlayerOfEvent(const std::string &eventName)
{
    ::getTopPlayerOfEvent(eventName, database.eventGames);
}

void QueryService::printAllEvents()
{
    ::printAllEvents(database.eventTree);
}

void QueryService::prettyPrintEventTree()
{
    database.eventTree.prettyPrint();
}

void QueryService::printEventsByYear(int year)
{
    ::getEventsByYear(database.eventTree, year);
}

void QueryService::printTotalGamesPlayedAtEvent(const std::string &eventName)
{
    ::getTotalGamesPlayedAtEvent(eventName, database.eventGames);
}

void QueryService::printEventsOfSameLocation(const std::string &location)
{
    ::getEventsOfSameLocation(database.eventTree, location);
}

void QueryService::printPlayersOfEvent(const std::string &eventName)
{
    ::printPlayerData(eventName, database.eventPlayers);
}

void QueryService::printOpeningVariations(const std::string &openingName)
{
    ::getVariationsOfOpenings(openingName, database.openingsTree);
}

void QueryService::printMatchupScore(const std::string &player1, const std::string &player2)
{
    ::getMatchupScore(player1, player2, database.playerGames);
}

void QueryService::printMatchupGames(const std::string &player1, const std::string &player2)
{
    ::getMatchupGames(player1, player2, database.playerGames);
}

void QueryService::printGamesOfPlayer(const std::string &playerName)
{
    ::printGameData(playerName, database.playerGames);
}

void QueryService::printMostPlayedOpeningByPlayer(const std::string &playerName)
{
    ::getMostPlayedOpening(playerName, database.playerGames);
}

void QueryService::printMostPlayedMoveAfterPgn(const std::string &pgn)
{
    ::getMostPlayedMove(pgn, database.gameTree);
}

void QueryService::printPlayerGamesByYear(const std::string &playerName, int year)
{
    ::getPlayerGamesByYear(playerName, year, database.playerGames);
}

void QueryService::printMostPlayedOpeningByEvent(const std::string &eventName)
{
    ::getMostPlayedOpening(eventName, database.eventGames);
}

void QueryService::filterGamesByAverageRating(int averageRating)
{
    ::filterGamesByRating(averageRating, database.gameTree);
}

void QueryService::filterGamesByMovesUpperBound(int totalMoves)
{
    ::filterGamesByMoves(totalMoves, database.gameTree);
}
