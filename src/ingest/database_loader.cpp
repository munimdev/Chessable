#include "ingest/database_loader.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

#include "Parser.h"

namespace
{
    int extractMaximum(const std::string &str)
    {
        int num = 0;
        int result = 0;
        for (std::size_t index = 0; index < str.size(); ++index)
        {
            if (str[index] >= '0' && str[index] <= '9')
            {
                num = num * 10 + static_cast<int>(str[index] - '0');
            }
            else if (str[index] == '.')
            {
                result = std::max(result, num);
                num = 0;
            }
            else
            {
                num = 0;
            }
            if (index == (str.size() - 1))
            {
                num = 0;
            }
        }
        return std::max(result, num);
    }
}

bool loadChessDatabase(
    const ChessableConfig &config,
    ChessDatabase &database,
    std::ostream &out,
    std::ostream &err
)
{
    out << '\n' << "Loading the chess data set." << std::endl;

    std::ifstream pgnFile(config.pgnPath);
    if (!pgnFile.is_open())
    {
        err << "Failed to open PGN dataset: " << config.pgnPath << std::endl;
        return false;
    }

    pgn::GameCollection games;
    pgnFile >> games;
    out << '\n' << "The dataset contains " << games.size() << " games." << std::endl << std::endl;

    std::ifstream openingDataset(config.openingsPath, std::ifstream::in);
    if (!openingDataset.is_open())
    {
        err << "Failed to open openings dataset: " << config.openingsPath << std::endl;
        return false;
    }

    std::string openingName;
    std::string ecoCode;
    int moveNumber = 0;
    char dot = '.';
    std::string whiteMove;
    std::string blackMove;
    std::string pgn;

    openingDataset.peek();
    while (openingDataset.good())
    {
        getline(openingDataset, ecoCode, '\t');
        AVLNode<ECO> *ecoNode = database.ecoTree.find(ECO(ecoCode));
        if (ecoNode == nullptr)
        {
            ecoNode = database.ecoTree.insertAndGet(ECO(ecoCode));
        }

        getline(openingDataset, openingName, '\t');
        getline(openingDataset, pgn);

        const int totalMoves = extractMaximum(pgn);
        AVLNode<OpeningNode> *openingNode = database.openingsTree.insertAndGet(OpeningNode{openingName, totalMoves});
        std::stringstream moveStream(pgn);
        while (moveStream >> moveNumber >> dot >> whiteMove)
        {
            openingNode->data.setofMoves[moveNumber - 1].moveNumber = moveNumber;
            openingNode->data.setofMoves[moveNumber - 1].move[0] = whiteMove;
            if (moveStream >> blackMove)
            {
                openingNode->data.setofMoves[moveNumber - 1].move[1] = blackMove;
            }
            else
            {
                openingNode->data.setofMoves[moveNumber - 1].move[1] = "";
            }
        }
        ecoNode->data.addOpening(openingNode->data);
        openingNode->data.belongsToECO = &ecoNode->data;
        openingDataset.peek();
    }

    openingDataset.close();

    for (auto itr = games.begin(); itr != games.end(); ++itr)
    {
        const bool makeNewEvent = (database.eventGames.getIndex(itr->tags()["Event"].value()) == -1);

        AVLNode<EventLocation> *eventLocationNode = database.eventLocationTree.find(itr->tags()["Site"].value());
        if (eventLocationNode == nullptr)
        {
            eventLocationNode = database.eventLocationTree.insertAndGet(itr->tags()["Site"].value());
        }

        AVLNode<EventDate> *eventDateNode = database.eventDateTree.find(itr->tags()["EventDate"].value());
        if (eventDateNode == nullptr)
        {
            eventDateNode = database.eventDateTree.insertAndGet(itr->tags()["EventDate"].value());
        }

        Event *eventPtr = nullptr;
        if (makeNewEvent)
        {
            AVLNode<Event> *eventNode = database.eventTree.insertAndGet(
                Event(itr->tags()["Event"].value(), &eventLocationNode->data, &eventDateNode->data)
            );
            eventPtr = &eventNode->data;
        }
        else
        {
            eventPtr = database.eventGames.hashTable[database.eventGames.getIndex(itr->tags()["Event"].value())]
                           .list.root->data->playedInEvent;
        }

        AVLNode<Player> *whitePlayerNode = database.playerTree.find(itr->tags()["White"].value());
        if (whitePlayerNode == nullptr)
        {
            whitePlayerNode = database.playerTree.insertAndGet(itr->tags()["White"].value());
        }
        Player *whitePlayerSidePtr = &whitePlayerNode->data;

        AVLNode<Player> *blackPlayerNode = database.playerTree.find(itr->tags()["Black"].value());
        if (blackPlayerNode == nullptr)
        {
            blackPlayerNode = database.playerTree.insertAndGet(itr->tags()["Black"].value());
        }
        Player *blackPlayerSidePtr = &blackPlayerNode->data;

        database.eventPlayers.doubleHashInsert(eventPtr->eventName, whitePlayerSidePtr);
        database.eventPlayers.doubleHashInsert(eventPtr->eventName, blackPlayerSidePtr);
        database.playerEvents.doubleHashInsert(whitePlayerSidePtr->playerName, eventPtr);
        database.playerEvents.doubleHashInsert(blackPlayerSidePtr->playerName, eventPtr);

        pgn::Game game = *itr;
        pgn::MoveList moveList = game.moves();

        AVLNode<ECO> *ecoNode = database.ecoTree.find(itr->tags()["ECO"].value());
        if (ecoNode == nullptr)
        {
            ecoNode = database.ecoTree.insertAndGet(itr->tags()["ECO"].value());
        }

        pgn = moveList.getPGN();
        Game *newGame = new Game(
            eventPtr,
            whitePlayerSidePtr,
            blackPlayerSidePtr,
            &ecoNode->data,
            itr->tags()["Result"].value(),
            stoi(itr->tags()["WhiteElo"].value()),
            stoi(itr->tags()["BlackElo"].value()),
            moveList.size(),
            pgn
        );

        if (itr->tags()["Result"].value() == "1-0")
        {
            whitePlayerSidePtr->wins++;
            blackPlayerSidePtr->losses++;
        }
        else if (itr->tags()["Result"].value() == "0-1")
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

        database.playerMatchups.doubleHashInsert(whitePlayerSidePtr->playerName, blackPlayerSidePtr);
        database.playerMatchups.doubleHashInsert(blackPlayerSidePtr->playerName, whitePlayerSidePtr);
        database.eventGames.doubleHashInsert(eventPtr->eventName, newGame);
        database.playerGames.doubleHashInsert(whitePlayerSidePtr->playerName, newGame);
        database.playerGames.doubleHashInsert(blackPlayerSidePtr->playerName, newGame);

        AVLNode<Game> *gameNode = database.gameTree.insertAndGet(*newGame);
        gameNode->data.playedInEvent = eventPtr;
    }

    return true;
}
