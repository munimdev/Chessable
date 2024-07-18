#include "app/config_parser.h"
#include "app/run_chessable_cli.h"

int main(int argc, char **argv)
{
    const ChessableConfig config = parseChessableConfig(argc, argv);
    return runChessableCli(config);
}
