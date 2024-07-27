#include <fstream>
#include <iostream>
#include "Parser.h"
#include "app/cli_runner.h"
#include "app/run_chessable_cli.h"
#include "domain/chess_database.h"
#include "ingest/database_loader.h"
#include "domain/chess_types.h"
#include "query/query_service.h"

using namespace std;

int runChessableCli(const ChessableConfig &config)
{
  std::ifstream pgnfile(config.pgnPath);
  pgn::GameCollection games;
  pgnfile >> games;
  ChessDatabase database(games.size());
  if(!loadChessDatabase(config, database, cout, cerr))
  {
    return 1;
  }
  QueryService queryService(database);
  return app::runInteractiveCli(queryService);
}