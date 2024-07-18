#include "app/config_parser.h"

#include <cstring>

ChessableConfig parseChessableConfig(int argc, char **argv)
{
    ChessableConfig config;

    for (int index = 1; index < argc; ++index)
    {
        if (std::strcmp(argv[index], "--pgn") == 0 && (index + 1) < argc)
        {
            config.pgnPath = argv[++index];
            continue;
        }

        if (std::strcmp(argv[index], "--openings") == 0 && (index + 1) < argc)
        {
            config.openingsPath = argv[++index];
            continue;
        }
    }

    return config;
}
