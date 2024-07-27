#include "app/cli_runner.h"

#include "app/menu_controller.h"
#include "cli/menu.h"

namespace app
{
    int runInteractiveCli(QueryService &queryService)
    {
        int choice = 0;
        do
        {
            cli::printMainMenu();
            choice = cli::readMenuChoice();
            if (!app::processMenuChoice(choice, queryService))
            {
                break;
            }
        } while (choice != -1);

        return 0;
    }
}
