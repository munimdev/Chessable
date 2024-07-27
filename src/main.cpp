#include <fstream>
#include <iostream>
#include "Parser.h"
#include "app/menu_controller.h"
#include "app/run_chessable_cli.h"
#include "cli/menu.h"
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
  
  int choice;
  do {
    cli::printMainMenu();
    choice = cli::readMenuChoice();
    if (!app::processMenuChoice(choice, queryService))
    {
      break;
    }
  } while (choice != -1);

  return 0;
}