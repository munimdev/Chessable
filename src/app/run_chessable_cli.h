#pragma once

struct ChessableConfig
{
    const char *pgnPath = "./WorldCup2021.pgn";
    const char *openingsPath = "pgndata.txt";
};

int runChessableCli(const ChessableConfig &config);
