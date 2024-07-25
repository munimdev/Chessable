#pragma once

#include <algorithm>

#include "domain/chess_types.h"

struct ChessDatabase
{
    AVLTree<EventDate> eventDateTree;
    AVLTree<EventLocation> eventLocationTree;
    AVLTree<Event> eventTree;

    AVLTree<Player> playerTree;
    AVLTree<Game> gameTree;
    AVLTree<ECO> ecoTree;
    AVLTree<OpeningNode> openingsTree;

    HashTable<Player *> eventPlayers;
    HashTable<Game *> eventGames;
    HashTable<Game *> playerGames;
    HashTable<Player *> playerMatchups;
    HashTable<Event *> playerEvents;

    static int scaledBuckets(int gameCount, int divisor, int minimum)
    {
        return std::max(minimum, (gameCount / divisor) + 1);
    }

    explicit ChessDatabase(int gameCount)
        : eventPlayers(scaledBuckets(gameCount, 10, 101)),
          eventGames(scaledBuckets(gameCount, 10, 101)),
          playerGames(scaledBuckets(gameCount, 4, 257)),
          playerMatchups(scaledBuckets(gameCount, 4, 257)),
          playerEvents(scaledBuckets(gameCount, 4, 257))
    {
    }
};
