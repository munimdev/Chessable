#include "menu.h"

#include <iostream>

namespace cli
{
    void printMainMenu()
    {
        std::cout << "\n\x1b[36mPlease select an action (-1 to end): " << std::endl << std::endl;
        std::cout << "\t1. Get Openings by ECO           2. Events Attended By A Player" << std::endl;
        std::cout << "\t3. Get Winrate of a Player       4. Get Player Matchups" << std::endl;
        std::cout << "\t5. Get Games Of An Event         6. Get Top Player Of Event" << std::endl;
        std::cout << "\t7. Print All Event Data          8. Pretty Print Events" << std::endl;
        std::cout << "\t9. Filter Events By Year         10. Get Total Games Played At Event"
                  << std::endl;
        std::cout << "\t11. Filter Events By Location    12. Players That Attened Some Event"
                  << std::endl;
        std::cout << "\t13. Get Variations of Opening    14. Get Score Between Two Players"
                  << std::endl;
        std::cout << "\t15. Get All Games of a Matchup   16. Get All Games of a Player"
                  << std::endl;
        std::cout << "\t17. Get Most Played Opening      18. Get Most Played Move After A Position"
                  << std::endl;
        std::cout << "\t19. Filter Player Games By Year  20. Get Most Played Opening Of Event"
                  << std::endl;
        std::cout << "\t21. Filter Games By Rating       22. Filter Games By Moves" << std::endl;
    }

    int readMenuChoice()
    {
        int choice = 0;
        std::cout << "\x1b[0m";
        std::cin >> choice;
        std::cout << std::endl;
        return choice;
    }
}
