#pragma once

#include <ostream>

#include "app/run_chessable_cli.h"
#include "domain/chess_database.h"

bool loadChessDatabase(
    const ChessableConfig &config,
    ChessDatabase &database,
    std::ostream &out,
    std::ostream &err
);
