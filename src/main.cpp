#include <iostream> //standard i/o library
#include <fstream> //for file reading
#include <sstream> //for string stream
#include <string> //string library

#include "AVL.h" //include avl tree
#include "List.h" //include linked list
#include "Hash.h" //include hash table

#include "Parser.h" //PGN library parser
using namespace std;

class OpeningNode; //foward declaration of class OpeningNode as it is to be used be ECO class

//Encyclopaedia of Chess Openings (ECO) class stores the information about the EcoCode of a chess opening
class ECO 
{
  public:
    string EcoCode; //stores the ECO code of an opening
    LinkedList<OpeningNode*> belongsToOpening; //an eco can belong to many openings (check pgndata.txt), therefore a linked list is used

    ECO(string EcoCode="") //parametrized constructor
    {
      this->EcoCode = EcoCode;
    };

    //overloading stream insertion << operator
    //this helps to directly print an ECO object
    friend ostream &operator<<( ostream &output, const ECO& EcoObject) 
    {
      output << EcoObject.EcoCode;
      return output;
    };

    //overloading of boolean comparison operators for sotring data in AVL tree
    bool operator==(ECO &EcoObject)
    {
      return this->EcoCode == EcoObject.EcoCode;
    }

    bool operator!=(ECO &EcoObject)
    {
      return this->EcoCode != EcoObject.EcoCode;
    }

    bool operator>(ECO &EcoObject)
    {
      return this->EcoCode > EcoObject.EcoCode;
    }

    bool operator<(ECO &EcoObject)
    {
      return this->EcoCode < EcoObject.EcoCode;
    }
    
    void addOpening( OpeningNode &opening )
    {
      this->belongsToOpening.insert(&opening); // *
    }

    //class function definitions

    //displays all the openings of an eco object  
    //one function does not have any parameter as it calls the next function
    void getOpenings();
    void getOpenings( ListNode<OpeningNode*>* opening );

};

//Call the getOpening fnction which displays all the opening data of an ECO
void ECO::getOpenings()
{
  cout << this->EcoCode << " contains the following openings: " << endl;
  getOpenings(belongsToOpening.root);
}

//takes as input an Eco code
//searches the EcoCode within the AVLTree and prints all its opening if its found 
void getOpenings(string EcoCode, AVLTree<ECO> &ecoTree)
{
  ecoTree.search(ECO(EcoCode)); //searches an EcoNode for the given EcoCode value
  //calls the getOpenings() function if the node was found in the tree
  if(ecoTree.loc)
  {
    ecoTree.loc->data.getOpenings(); //prints all the openings of an Eco object
  }
  else
    cout << "\e[0;31mThat ECO code does not exist!\x1b[0m" << endl;
} 

class EventLocation //class EventLocation stores information for the location of an event
{ 
  public:
    string eventLocation; //stores location of an event

  EventLocation(string eventLocation="") //constructor
  {
    this->eventLocation = eventLocation;
  }

  //operator << overloading for printing an EventLocation object
  friend ostream& operator<<(ostream& output, const EventLocation& eventLocationObject)
  {
    output << eventLocationObject.eventLocation; //insert the location into the output stream
    return output; //return the output stream
  };

  //overload comparison ooperators for comparison of two EventLocation objects
  //also helps for storing objects into an AVL Tree or Linked List
  friend bool operator==(EventLocation &object1,EventLocation &object2)
  {
    return object1.eventLocation == object2.eventLocation;
  }
  friend bool operator<(EventLocation &object1,EventLocation &object2)
  {
    return object1.eventLocation < object2.eventLocation;
  }
  friend bool operator>(EventLocation &object1,EventLocation &object2)
  {
    return object1.eventLocation > object2.eventLocation;
  }
};

class EventDate { //stores date for an event
  public: 
    //stores the year, month and day for an event
    //as month an year have less than 256 possible values, we are using the char datatype to store month and day for minimum storage usage
    unsigned short year;
    unsigned char month; 
    unsigned char day;

    EventDate(unsigned short day=0, unsigned char month=0, unsigned char year=0) //parametrized constructor that directly takes the year, month, and day values
    {
      this->day = day;
      this->month = month;
      this->year = year;
    }

    EventDate(string date) //overloaded constructor in case a string is passed
    //the date is passed in the format "YYYY.MM.DD"
    {
      if(date.substr(0, date.find(".")) != "????") //get the substring until the seperator and save it in the year unless it is an undefined date
        this->year = stoi(date.substr(0, date.find("."))); //convert string into integer before saving
      else
        this->year = 0;
      date.erase(0, date.find_first_of(".")+1); //now discard the portion of the string that contains t he year
      if(date.substr(0, date.find(".")) != "??") //repeat process for month and day
        this->month =  stoi(date.substr(0, date.find(".")));
      else
        this->month = 0;
      date.erase(0, date.find_first_of(".")+1);
      if(date.substr(0, date.find(".")) != "??" )
        this->day = stoi(date.substr(0, date.find(".")));
      else
        this->day = 0;
    }
 
    //stream insertion operator for for printing an EventDate object
    friend ostream& operator<<(ostream& output, const EventDate& eventDateObject)
    {
      output << eventDateObject.year << '.' << int(eventDateObject.month) << '.' << int(eventDateObject.day);
      return output;
    };

    //compares two date nodes using ==
    //two date objects are equal if and only if their respective day, month, and year are equal
    friend bool operator==(EventDate &object1,EventDate &object2)
    {
      return object1.day == object2.day && object1.month == object2.month && object1.year == object2.year;
    }

    //compares 2 dates using <
    //if object1 is <, true is returned
    friend bool operator<(EventDate &object1,EventDate &object2)
    {
      if(object1.year < object2.year) //returns true if year object1 has a lower year value
        return true;
      else if(object1.year == object2.year) //else if both have the same year, check for the month value
      {
        if(object1.month < object2.month) //returns true if year object1 has a lower month value
          return true;
        else if(object1.month == object2.month) //else if both have the same year, check for the day value
        {
          if(object1.day < object2.day) //returns true if year object1 has a lower month value
            return true;
          else
            return false; //return false as object1 did not have a lower date
        }
        else
          return false; //return false as object1 did not have a lower date
      }
      else
        return false; //return false as object1 did not have a lower date
    }

    //compares 2 dates using >
    //if object1 is >, true is returned
    friend bool operator>(EventDate &object1,EventDate &object2)
    {
      if(object1.year > object2.year)
        return true;
      else if(object1.year == object2.year)
      {
        if(object1.month > object2.month)
          return true;
        else if(object1.month == object2.month)
        {
          if(object1.day > object2.day)
            return true;
          else
            return false;
        }
        else
          return false;
      }
      else
        return false;
    }
};


class Game; //forward definition for class Game

class Player  //stores Information of a chess player
{ 
  public:  
    string playerName; //stores name of the player
    int totalGames, wins, draws, losses; //stores the players lifetime record of wins, draws, losses and total games

    Player(string playerName = "") //default contructor that also acts as a paramterized constructor if playerName is passed
    {
      this->playerName = playerName;
      this->wins = 0;
      this->draws = 0;
      this->losses = 0;
      this->totalGames = 0;
    }

    //<< operator overloading for printing purposes
    friend ostream &operator<<( ostream &output, const Player& P)
    {
      output << P.playerName;
      return output;
    };

    //overloading
    friend bool operator==(Player &, Player &);
    friend bool operator<(Player &, Player &);
    friend bool operator>(Player &, Player &);

    void mostPlayedOpening(); // Prints/Returns a player’s favorite repertoire of openings

    void getWinrate(); //prints a player's lifetime win-rate of chess games
};

void Player::getWinrate() //prints a player's lifetime win-rate of chess games
{
  cout << "Win rate of " << playerName << " is: " << 100*double(wins + double(draws/2))/(totalGames) << "%";
}

void getPlayerMatchups(string playerName, HashTable<Player*>&playerTable) //prints the list of all the players a player has played a chess match against
{
  int playerIndex = playerTable.getIndex(playerName); //get the hash table index by hashing the player name
  if (playerIndex != -1)
  {
    cout << playerName << " has played against the following players: " << endl << endl;
    auto itr = playerTable.hashTable[playerIndex].list.begin(); //define an iterator for the linked list stored at the hashed index
    
    //traverse the given linked list and print each player one by one
    for(; itr != playerTable.hashTable[playerIndex].list.end(); itr++)
    {
      cout << *itr.currentNode->data << " - ";
    }
  }
  else
    cout << "\e[0;31mThat player does not exist in our database!\x1b[0m" << endl;
}

//compares 2 player objects using == overloading
//if both have same names, they are the same player
bool operator==(Player &object1, Player & object2)
{
  if (object1.playerName == object2.playerName)
    return true;
  else
    return false;
}

//compares 2 players using < and > oberloading
//this is needed as all players as stores in an AVLTree
//comparison is done on the basis of the player name
bool operator<(Player &object1, Player & object2)
{
  if (object1.playerName < object2.playerName)
    return true;
  else
    return false;
}

bool operator>(Player &object1, Player & object2)
{
  if (object1.playerName > object2.playerName)
    return true;
  else
    return false;
}

class Event { //stores the information for an event
  public: 
    string eventName; //stores the name of an event
    EventLocation *eventLocation; //points to an event location
    EventDate *eventDate; //points to an event date  
    //the reason we used pointers for event location and event date is because a single character in a string takes 1 byte space. So an average event location will take roughly 15 bytes of space whereas the pointer just
    
    Event(string eventName="", EventLocation *eventLocation=nullptr, EventDate *eventDate=nullptr) //parametrized + default constructor constructor
    {
      this->eventName = eventName;
      this->eventLocation = eventLocation;
      this->eventDate = eventDate;
    }

    //<< operator is overloaded for the Event class for printing purposes
    //the output is returned in PGN format
    friend ostream &operator<<( ostream &output, const Event& E)
    {
      output << "[Event " << "\"" << E.eventName << "\"]"<< endl;
      output << "[Site " << "\"" << *E.eventLocation << "\"]" <<  endl;
      output << "[Date " << "\"" << *E.eventDate << "\"]" << endl;
      return output;
    };  

  //function definitions for operator overloading
  friend bool operator==(Event &object1, Event & object2);
  friend bool operator<(Event &object1, Event & object2);
  friend bool operator>(Event &object1, Event & object2);

};

//outputs the total number of games played at a certain event
//takes as input the event name and a reference to the hashtable that contains a list of games
void getTotalGamesPlayedAtEvent(string eventName, HashTable<Game*> &gameTable)
{
  int eventIndex = gameTable.getIndex(eventName); //find the hashed index of the event and check if it exists
  if( eventIndex != -1 )
    cout << "Total games played at " << eventName << " were: " << gameTable.hashTable[eventIndex].list.size; //print the list size 
  else
    cout << "\e[0;31mThat event does not exist in our database!\x1b[0m" << endl;
}

//outputs all the events played at a certain location
//takes as input the location and a pointer to an AVLNode that stores Event data
//Uses inOrder recursive traversal and prins the event data if its location matches the input location
void getEventsOfSameLocation(AVLNode<Event>* eventNode, string location)
{
  if(eventNode) //check if node is valid
  {
    getEventsOfSameLocation(eventNode->left, location); //explore left branch
    if(eventNode->data.eventLocation->eventLocation == location ) //print node data if location matches
      cout << eventNode->data << endl;
    getEventsOfSameLocation(eventNode->right, location); //epxlore right branch
  }
}

//outputs all the events played on a certain year
//takes as input the year and a pointer to an AVLNode that stores Event data
//Uses inOrder recursive traversal and prints the event data if its year matches the input year
void getEventsByYear(AVLNode<Event>* eventNode, int year)
{
  if (eventNode) //check if node is valid
  {
    getEventsByYear(eventNode->left, year);
    if(eventNode->data.eventDate->year == year)
      cout << eventNode->data << endl;
    getEventsByYear(eventNode->right,  year);
  }
}

void printAllEvents( AVLNode<Event> *eventNode ) //display the events in alphabetical order or sorted according to their date
{
  if(eventNode) //base case of recursive function
  {
    printAllEvents(eventNode->left);
    cout << eventNode->data << endl; //print event data
    printAllEvents(eventNode->right);
  }
}

//if two events have the same name, then they must be the same Event (events can have similar locations or dates, but never names)
bool operator==(Event &object1, Event &object2)
{
  if (object1.eventName == object2.eventName)
    return true;
  else
    return false;
}

//< and > were overloaded for comparison purposes for insertion in AVL tree
bool operator<(Event &object1, Event & object2)
{
  if (object1.eventName < object2.eventName)
    return true;
  else
    return false;
}

bool operator>(Event &object1, Event & object2)
{
  if (object1.eventName > object2.eventName)
    return true;
  else
    return false;
}

//prints the data for all the players that attended a certain event
//takes as input the event name and a reference to the hashtable that contains a list of event players
void printPlayerData(string eventName, HashTable<Player*> &eventPlayersTable)
{
  int eventIndex = eventPlayersTable.getIndex(eventName); //get the hashed index of the given event name
  if (eventIndex != -1) //if the event exists in the hash table, simply iterator over its player list and print them
  {
    cout << "Players that played in " << eventName << " were: " << endl;
    auto itr = eventPlayersTable.hashTable[eventIndex].list.begin();
    for(; itr != eventPlayersTable.hashTable[eventIndex].list.end(); itr++)
    {
      cout << itr.currentNode->data->playerName << " - ";
    }
  }
  else
    cout << "\e[0;31mThat event does not exist in our database!\x1b[0m" << endl;
}

//prints the data of all the events that a player attended
//takes as input the player name
//hashes the player name and prints the list, otherwise outputs that the given played does not exist in the database
void printEventData(string playerName, HashTable<Event*> &playerEventTable)
{
  int playerIndex = playerEventTable.getIndex(playerName);
  if (playerIndex != -1)
  {
    cout << "Events attended by " << playerName << " are: " << endl << endl;
    auto itr = playerEventTable.hashTable[playerIndex].list.begin(); //list iterator
    for(; itr != playerEventTable.hashTable[playerIndex].list.end(); itr++) //print list
    {
      cout << *itr.currentNode->data << endl;
    } 
  }
  else
    cout << "\e[0;31mThat player does not exist in our database!\x1b[0m" << endl;
}

class MoveNode {
  public:
    array<string, 2> move; //0th index stores the move of player white
    //1st index stores the move of player black

    int moveNumber; //tells the number the move was player at e.g. "1. e4 e5"
    //here white played e4 and black played e5 on move 1
  
    // Default constructor
    MoveNode() 
    {
      this->move.fill("\0"); 
    }
    
    //parametrized constructor to initialize the data members if they are passed seoerate
    MoveNode(int moveNumber, string whiteMove, string blackMove)
    {
      this->move[0] = whiteMove;
      this->move[1] = blackMove;
      this->moveNumber = moveNumber;
    }

    //parametrized constructor that parses a single move string
    //e.g. "1. e4 e5"
    MoveNode(string move) 
    {
      this->moveNumber = stoi(move.substr(0, move.find("."))); 
      this->move[0] = move.substr(move.find_first_of(" ")+1, move.find_last_of(" ")-move.find_first_of(" ")-1);
      this->move[1] = move.substr(move.find_last_of(" ")+1);
    }

    //Overloading [] operator to access moves of white side or black side
    //e.g. move["White"] will return white's move
    string& operator [] (string& key)
    {
      if(key == "White")
        return move[0];
      if(key == "Black")
        return move[1]; 

      throw runtime_error("Invalid key inputted."); //throw a runtime error if user enters an invalid key
    }
    
    //Overloading << operator 
    //it prints a single set of a move in PGN format
    friend ostream &operator<<( ostream &output, const MoveNode& M)
    {
      output << M.moveNumber << ". " << M.move[0] << " " << M.move[1] << " ";
      return output;
    };
    
    //function definitions for comparison operator overloadiing
    friend bool operator>(MoveNode&, MoveNode&);
    friend bool operator<(MoveNode&, MoveNode&);
};

//< > operators are overloaded for storing MoveNodes in an AVL tree
bool operator<(MoveNode& object1, MoveNode& object2)
{
  if (object1.moveNumber < object2.moveNumber)
    return true;
  else
    return false;
}

bool operator>(MoveNode& object1, MoveNode& object2)
{
  if (object1.moveNumber > object2.moveNumber)
    return true;
  else
    return false;
}


//definition for clas Game
//it stores all he information related to a chess game
class Game
{
  public:
    Player *whitePlayerSide; //points towards white player side
    Player *blackPlayerSide; //black side pointer
    string result; //stores result of the game - 1-0 means white won, 0-1 means black won. 1/2-1/2 means it was a draw
    int numberOfMoves; //stores total number of moves played in the game (1 move means 1 black move, 1 white move)
    OpeningNode *openingPlayed;
    ECO *ECO;
    MoveNode *setofMoves; //an array of type MoveNode. It stores all the moves that were played in a game
    int whiteElo; //stores the rating of white player side
    int blackElo; //stores the rating of black play+er side
    Event *playedInEvent; //points to the event the game was played at
    
    //default constructor
    Game()
    {
      whitePlayerSide = NULL;
      blackPlayerSide = NULL;
      result = "";
      whiteElo = 0;
      blackElo = 0;
    }

    //parameterized constructor
    Game(Event *eventPtr, Player *whitePlayerSide, Player *blackPlayerSide, class ECO *EcoPtr, string result,  int whiteElo, int blackElo, int totalMoves, string moves)
    {
      this->playedInEvent = eventPtr; //saves the address of the event
      this->whitePlayerSide = whitePlayerSide; //saves the address of the white side
      this->blackPlayerSide = blackPlayerSide; //saves the address of the white side
      this->ECO = EcoPtr; //saves a pointer
      this->result = result;  //saves game result
      this->whiteElo = whiteElo; //saves rating of player white
      this->blackElo = blackElo; //saves rating of player black
      this->numberOfMoves = totalMoves; //save total moves of the game
      this->setofMoves = new MoveNode[numberOfMoves]; //dynamically create a move array based on total moves
      
      //parse the input PGN and store each move and move number individually in the array
      int move_number;
      char dot;
      string whiteMove; //temporarily stores player white move
      string blackMove; //temporarily stores player black move
      stringstream moveStream(moves); //create a string stream of the moves

      //loop until end of stream is reached and store data in the respectice array indexes
      while(moveStream >> move_number >> dot >> whiteMove)
      {
        setofMoves[move_number-1].moveNumber = move_number;
        setofMoves[move_number-1].move[0] = whiteMove;
        if(moveStream >> blackMove)
          setofMoves[move_number-1].move[1] = blackMove;
        else
          setofMoves[move_number-1].move[1] = "\0";
      }
    }

    //Overloading comparison operators for storing storing in an AVL tree
    //Overload < operator
    friend bool operator<(Game& G1, Game& G2)
    {
      if (G1.whitePlayerSide->playerName+G1.blackPlayerSide->playerName < G2.whitePlayerSide->playerName+G2.blackPlayerSide->playerName )
        return true;
      else
        return false;
    }

    //Overload > operator
    friend bool operator>(Game& G1, Game& G2)
    {
      if (G1.whitePlayerSide->playerName+G1.blackPlayerSide->playerName > G2.whitePlayerSide->playerName+G2.blackPlayerSide->playerName)
       return true;
     else
       return false;
    }

    //Overload == operator
    friend bool operator==(Game& G1, Game& G2)
    {
      if (G1.result == G2.result && G1.whiteElo == G2.whiteElo && G1.blackElo == G2.blackElo && G1.numberOfMoves == G2.numberOfMoves && G1.whitePlayerSide->playerName == G2.whitePlayerSide->playerName && G1.blackPlayerSide->playerName == G2.blackPlayerSide->playerName)
       return true;
     else
       return false;
    }

    //outputs a game object in PGN format to the ostream
    friend ostream &operator<<( ostream &output, const Game& game)
    {
      output << *game.playedInEvent;
      output << "[White \"" << game.whitePlayerSide->playerName << "\"]" << endl;
      output << "[Black \"" << game.blackPlayerSide->playerName << "\"]" << endl;
      output << "[Result \"" << game.result << "\"]" << endl;
      output << "[ECO \"" << game.ECO->EcoCode << "\"]" << endl;
      output << "[WhiteElo \"" << game.whiteElo << "\"]" << endl;
      output << "[BlackElo \"" << game.blackElo << "\"]" << endl << endl;
      output << game.getPGN() << endl; //call the getPGN() function which outputs all the moves of the game in PGN format
      return output;
    };

    string getPGN() const //returns all the moves of an opening in PGN format
    {
      stringstream PGN;
      for(int i=0; i< this->numberOfMoves; i++)
      {
        PGN << this->setofMoves[i];
      }
      return PGN.str();
    }

    string getPGN(int moves) const //returns all the moves of an opening in PGN format uptil the input move number
    {
      stringstream PGN;
      for(int i=0; i< moves; i++)
      {
        PGN << this->setofMoves[i]; 
      }
      return PGN.str();
    } 

    //function definitions for the Game class
    void displayTopRatedGames(); // Information (including all the moves in PGN format) of X top-rated games
    void getGamesByNumberOfMoves(int totalMoves); // print games that ended before the entered number of moves
    void getMostPlayedMove(string PGN); // This function will search all games that consists of the input opening moves. It will then return the most popular set of moves 

};

//Filter all games whose average rating is greater than entered rating
void filterGamesByRating(int averageRating, AVLNode<Game> *gameNode)
{
  if (gameNode)
  {
    filterGamesByRating (averageRating, gameNode->left);
    if (averageRating < ((gameNode->data.whiteElo + gameNode->data.blackElo)/2))
    {
      cout << gameNode->data << endl;
    }
    filterGamesByRating (averageRating, gameNode->right);
  }
}

//Filter all games whose total moves are less than the entered number of moves
//takes as input the total moves
//recursive function where avlnode is passed and all games are traversed and printed only if their total moves have an upper bound of input
void filterGamesByMoves(int totalMoves, AVLNode<Game> *gameNode)
{
  if (gameNode) //base case
  {
    filterGamesByMoves(totalMoves, gameNode->left); //explore left subtree
    if (totalMoves >= gameNode->data.numberOfMoves) //check upper bound
    {
      cout << gameNode->data << endl;
    }
    filterGamesByMoves(totalMoves, gameNode->right); //explore right subtree
  }
}

//Filter a player's games played in certain year
//takes as input the player name and the year
//hash table of player games is passed as a parameter
void getPlayerGamesByYear(string playerName, int year, HashTable<Game*> &playerGames)
{
  int index = playerGames.getIndex(playerName); //get hashed index of the player
  if (index != -1) //only filter games if the player exists
  {
    cout << playerName << " has the following games in the year " << year << endl << endl;

    //iterate over player games
    for (auto itr = playerGames.hashTable[index].list.begin(); itr != playerGames.hashTable[index].list.end(); itr ++ )
    {
      if (itr.currentNode->data->playedInEvent->eventDate->year == year ) //print them if the year matches
      {
        cout << *itr.currentNode->data << endl;
      }
    }
  }
  else
    cout << "\e[0;31mThat player does not exist in our database!\x1b[0m" << endl;
}

//outputs the highest rated player of an event
//takes as input the event name
//hash table of event games is passed as a parameter
void getTopPlayerOfEvent(string eventName, HashTable<Game *> &eventGames)
{
  Player *tempPlayerNode; //createt a temporary player pointer
  int highestELO=0; //store temporary highestELO rating 

  int eventIndex = eventGames.getIndex(eventName); //get index of the event name
  if(eventIndex != -1) //if the inputted event exists
  {
    //iterate over the games of the event
    for(auto iterator = eventGames.hashTable[eventIndex].list.begin(); iterator != eventGames.hashTable[eventIndex].list.end(); iterator++)
    {
      //if rating of player white is more than the current stored highest ELo
      //save the address of the whitePlayer side and update the highestELO variable
      if( iterator.currentNode->data->whiteElo > highestELO )
      {
        highestELO = iterator.currentNode->data->whiteElo;
        tempPlayerNode = iterator.currentNode->data->whitePlayerSide;
      }
      //same process for blackPlayer side
      if( iterator.currentNode->data->blackElo > highestELO )
      {
        highestELO = iterator.currentNode->data->blackElo;
        tempPlayerNode = iterator.currentNode->data->blackPlayerSide;
      }
    }
    cout << "The highest rated player of " << eventName << " is " << *tempPlayerNode << " with a rating of " << highestELO << "."; 
  }
  else
    cout << "\e[0;31mThat Event does not exits in our database!\x1b[0m" << endl;
}

//outputs all the games of an event
//takes as input the eventName
//the hashTable of event game is passed as a parameer
void getGamesOfEvent(string eventName, HashTable<Game*> &gameTable)
{
  int eventIndex = gameTable.getIndex(eventName);
  if (eventIndex != -1) //check if event exists
  {
    cout << "Games played at " << eventName << " are: " << endl;
    //iterate over games of event and output them
    auto itr = gameTable.hashTable[eventIndex].list.begin();
    for(; itr != gameTable.hashTable[eventIndex].list.end(); itr++)
    {
      cout << *itr.currentNode->data << endl;
    }
  }
  else
    cout << "\e[0;31mThat event does not exist in our database!\x1b[0m" << endl;
}

//class definition for class Opening
//stores all the information related to a chess opening
class OpeningNode 
{
  public:
    string openingName; //stores the name of an opening
    MoveNode *setofMoves; //an array of type MoveNode. It stores all the moves that were played in a game
    ECO *belongsToECO; //tells the eco code of the opening
    int totalMoves; //identifies total moves of an opening

    // Parametrized Constructor
    OpeningNode(string openingName = "")
    {
      this->openingName = openingName;
    }
    
    // Parametrized Constructor
    OpeningNode(string openingName, int totalMoves)
    {
      this->openingName = openingName;
      this->totalMoves = totalMoves;
      setofMoves = new MoveNode[totalMoves];  
    }

    //function definitions for comparison operator overloading
    friend bool operator>(OpeningNode&, OpeningNode&);
    friend bool operator<(OpeningNode&, OpeningNode&);
    friend bool operator==(OpeningNode&, OpeningNode&);

    string getPGN() const //returns all the moves of an opening in PGN format
    {
      stringstream PGN; //create a string stram object
      for(int i=0; i< this->totalMoves; i++)
      {
        //insert each move node one by one into the PGN
        PGN << this->setofMoves[i];
      }
      return PGN.str(); //return the string stream as a string
    }

    //Overload << for printing purposes
    friend ostream &operator<<( ostream &output, const OpeningNode& opening) 
    {
      output << "\e[0;34mOpening name\x1b[0m :: " << opening.openingName << endl << "\e[0;34mPGN\x1b[0m :: ";
      output << opening.getPGN();
      return output;
    };
  
};

//This functional will go through the list of chess openings and find all the variations that the inputted opening has
//Take as input the opening name
//revursive function
void getVariationsOfOpenings(string openingName, AVLNode<OpeningNode> *openingNode)
{
  if(openingNode)
  {
    getVariationsOfOpenings(openingName, openingNode->left); //explore left branch
    //if the input opening matches with a variation, print it
    if( openingNode->data.openingName.substr(0, openingNode->data.openingName.find(":")) == openingName )
      cout << openingNode->data << endl;
    getVariationsOfOpenings(openingName, openingNode->right); //explore right branch
  }
}

//outputs all the openings of an ECO object 
//as all the openings are stored in a linked list, we just trverse it and print it
void ECO::getOpenings(ListNode<OpeningNode *> *opening)
{
  if(opening) //if the node is valid
  {
    cout << *opening->data << endl; //print opening data
    getOpenings(opening->next); 
  }
  if(opening == this->belongsToOpening.root)
    cout << endl;
}

//< > operators are overloaded for storing MoveNodes in an AVL tree
//function definition for < operator
bool operator<(OpeningNode& object1, OpeningNode& object2)
{
  return object1.openingName < object2.openingName;
}

//function definition for > operator
bool operator>(OpeningNode& object1, OpeningNode& object2)
{
  return object1.openingName > object2.openingName;
}

//function definition for == operator
bool operator==(OpeningNode& object1, OpeningNode& object2)
{
  return object1.openingName == object2.openingName;
}

// Function to extract the maximum integer value within a string
//helps to find the total number of moves within a PGN string
int extractMaximum(string str)
{
    int num = 0, res = 0;
    // Start traversing the given string
    for (int i = 0; i<str.length(); i++)
    {
        // If a numeric value comes, start converting
        // it into an integer till there are consecutive
        // numeric digits
        if (str[i] >= '0' && str[i] <= '9')
            num = num * 10 + (str[i]-'0');
  
        // Update maximum value
        else if(str[i] == '.')
        {
            res = max(res, num);  
            // Reset the number
            num = 0;
        }
        else {
          num=0;
        }
        if( i == str.length()-1 ) //if te number appeared at the end of the string as a move value e.g. "3. Bg4", discard it as it is not a move number
          num=0;
    }
    // Return maximum value
    return max(res, num);
}


//takes as input two player names and iterates over their games to get their lifetime result against each other
//playerGames hash table is passed as all player 
void getMatchupScore(string player1, string player2, HashTable<Game *> &playerGames)
{
  int player1_GameIndex = playerGames.getIndex(player1); //get index of both players
  int player2_GameIndex = playerGames.getIndex(player2);

  if(player1_GameIndex != 1 && player2_GameIndex != -1) //only iterate over games if both player
  {
    double player1_score = 0, player2_score = 0; //stores scores of each player

    for(auto game_itr = playerGames.hashTable[player1_GameIndex].list.begin(); game_itr != playerGames.hashTable[player1_GameIndex].list.end(); game_itr++) //iterate over player games
    {
      //check if either of two player sides were a combination of the two input players
      if( (game_itr.currentNode->data->whitePlayerSide->playerName == player1 || game_itr.currentNode->data->blackPlayerSide->playerName == player1) && (game_itr.currentNode->data->blackPlayerSide->playerName == player2 || game_itr.currentNode->data->whitePlayerSide->playerName == player2) )
      {
        //if the combination of two player is valid
        //then simply update player scores based on their playing side and game result
        if(game_itr.currentNode->data->whitePlayerSide->playerName == player1) 
        {
          if( game_itr.currentNode->data->result == "1-0" )
            player1_score += 1;
          else if( game_itr.currentNode->data->result == "0-1" )
            player2_score += 1;
          else {
            player1_score += 0.5;
            player2_score += 0.5;
          }
        }
        else
        {
          if( game_itr.currentNode->data->result == "1-0" )
            player2_score += 1;
          else if( game_itr.currentNode->data->result == "0-1" )
            player1_score += 1;
          else {
            player1_score += 0.5;
            player2_score += 0.5;
          }
        }
      }
    }
    cout << player1 << " " << player1_score << "-" << player2_score << " " << player2 << endl;
  }
  else
    cout << "\e[0;31mEither of those player does not exits in our database!\x1b[0m" << endl;
}

//takes as input two player names and hash table of player games
//outputs the lifetime score of 2 players based on the result of al the games they played together
void getMatchupGames(string player1, string player2, HashTable<Game *> &playerGames)
{
  int player1_GameIndex = playerGames.getIndex(player1);
  int player2_GameIndex = playerGames.getIndex(player2);

  //if both players exist in data base, then iterate over games of player 1
  if(player1_GameIndex != 1 && player2_GameIndex != -1)
  {
    //loop for iteration
    for(auto game_itr = playerGames.hashTable[player1_GameIndex].list.begin(); game_itr != playerGames.hashTable[player1_GameIndex].list.end(); game_itr++)
    {
      //check if player 2 played against player1 in that game
      if((game_itr.currentNode->data->blackPlayerSide->playerName == player2 || game_itr.currentNode->data->whitePlayerSide->playerName == player2) )
      {
        //print the data of the game if the game was played by both player1 and player2
        cout << *game_itr.currentNode->data << endl << endl;
      }
    }
  }
  else
    cout << "\e[0;31mThat player does not exits\x1b[0m" << endl;
}

//outputs all the games of a player
//takes as input name of player1
//the hash table of player game is passed as a paramter
void printGameData(string player1, HashTable<Game *> &playerGames)
{
  int index = playerGames.getIndex(player1);
  if( index != -1) //check if player1 has games in the database
  {
    //iterate over the games
    for(auto itr = playerGames.hashTable[index].list.begin(); itr != playerGames.hashTable[index].list.end(); itr++)
    {
      //print the game data
      cout << *itr.currentNode->data << endl;
    }
  }
  else
    cout << "\e[0;31mThat player does not exist in our database!\x1b[0m" << endl;
}

//Traverse Hash table of games* & returns which openings were played the most
//takes as input either player name or event name
void getMostPlayedOpening(string userInput, HashTable<Game *> &games)
{
  int index= games.getIndex(userInput);
  if(index != -1) //check if games exist for the input even/player
  {
    //create a class OpeningInfo that stores the occurence of an opening and the pointer to its ECO
    class OpeningInfo
    {
      public:
        ECO * ecoPtr; //poins to eco of the opening
        int occurence; //stores the occurence of an opening

        OpeningInfo(ECO *ecoPtr = nullptr)
        {
          this->ecoPtr = ecoPtr;
          this->occurence=0;
        }

        //overloading == operator for storing OpeningInfo nodes in a linked list
        bool operator==(OpeningInfo &moveInfo)
        {
          return this->ecoPtr == moveInfo.ecoPtr;
        }
    };

    LinkedList<OpeningInfo> openingFrequency; //create a linked list of class OpeningInfo
    
    for( auto itr=games.hashTable[index].list.begin();  itr!=games.hashTable[index].list.end(); itr++) //iterate over the list of games and insert OpeningInfo objects
    {

      openingFrequency.insert(OpeningInfo(itr.currentNode->data->ECO)); //insert the opening into the list
      openingFrequency.search(OpeningInfo(itr.currentNode->data->ECO)); //get the address of the recent most insertion
      openingFrequency.loc->data.occurence++; //icnrement its occurence
    }

    auto highestOccurence = openingFrequency.root; //stores the address of the highest occuring openingInfo object
    
    for(auto itr=openingFrequency.begin(); itr != openingFrequency.end(); itr++) //iterate over the openingFrequency list
    {
      if( itr.currentNode->data.occurence > highestOccurence->data.occurence  ) //compare occurence of iterator , if itr has greater occurence
        highestOccurence = itr.currentNode; //u
    }

    cout << endl << "Most played opening of " << userInput << " is: " << endl;
    cout << "ECO Code: " << highestOccurence->data.ecoPtr->EcoCode << endl;
    cout << "Opening Name: " << highestOccurence->data.ecoPtr->belongsToOpening.root->data->openingName.substr(0, highestOccurence->data.ecoPtr->belongsToOpening.root->data->openingName.find(":")) << endl;
    cout << "Times Played: " << highestOccurence->data.occurence << endl;
  }
}

//stores pointers of all those games whose initial moves match the given pgn
void copyMatchingMove(string pgn, int totalMoves, bool blackMoved, AVLNode<Game> *gameNode, LinkedList<Game*> &matchingMoveList)
{
  if(gameNode && gameNode->data.numberOfMoves >= totalMoves ) //check if node is valid and the total moves played in a game were equal or greater
  {
    copyMatchingMove(pgn, totalMoves, blackMoved, gameNode->left, matchingMoveList); //explore left subtree

    //if black side did not move in input pgn, we must strip the black move from the game pgn as well
    string gamePGN = gameNode->data.getPGN(totalMoves);
    gamePGN = gamePGN.substr(0, gamePGN.find_last_of(" "));

    if(!blackMoved) //strip black move
      gamePGN = gamePGN.substr(0, gamePGN.find_last_of(" "));
    if(pgn == gamePGN) //if pgn is a match, insert into list
      matchingMoveList.insert(&gameNode->data);
    copyMatchingMove(pgn, totalMoves, blackMoved, gameNode->right, matchingMoveList); //explore right sub tree
  }
}

//outputs the most played after the inputted set of moves in PGN format

void getMostPlayedMove(string pgn, AVLNode<Game> *gameNode)
{
  int totalMoves = extractMaximum(pgn);
  LinkedList<Game*> matchingMoveList;

  class MoveInfo //create a class MoveInfo that stores the occurence of the move and the move data
  {
    public:
      string move;   //stores the move data
      int occurence; //stores the occurence of a move

      //default constructor
      MoveInfo()
      {
        this->occurence=0;
      }

      //parametrized constructor
      MoveInfo(string move)
      {
        this->move = move;
        this->occurence=0;
      }

      //overloading == operator for storing OpeningInfo nodes in a linked list
      bool operator==(MoveInfo &moveInfo)
      {
        return this->move == moveInfo.move;
      }
  };
  
  LinkedList<MoveInfo> moveFrequency; //stores all the moves that were played 

  bool blackMoved; //determines whether the black player side made a move in the input pgn or not
  string moveNum = to_string(totalMoves)+".";
  string tempPGN = pgn.substr(pgn.find(moveNum)+3);
  tempPGN.erase(0, tempPGN.find_first_of(" "));
  if (tempPGN.find(" ") == string::npos)
    blackMoved = 0;
  else
    blackMoved = 1;

  copyMatchingMove(pgn, totalMoves, blackMoved, gameNode, matchingMoveList);  //copies all the matching game pointers into the matchingMoveList whose PGN uptil totalMoves is same as the input PGN

  for( auto itr=matchingMoveList.begin();  itr!=matchingMoveList.end(); itr++) //iterate over the matchingMoveList
  {
    if(blackMoved) //if black made a move, we have to find the white move as next move
    {
      moveFrequency.insert(MoveInfo(itr.currentNode->data->setofMoves[totalMoves].move[0])); //inserts the next move into the moveFrequency
      if(moveFrequency.search(MoveInfo(itr.currentNode->data->setofMoves[totalMoves].move[0])))
      {
        moveFrequency.loc->data.occurence++; //increase occurence
      }
    }
    else //else if black didn't move, we have to find the most played black move after the position
    {
      moveFrequency.insert(MoveInfo(itr.currentNode->data->setofMoves[totalMoves-1].move[1])); //inserts the next move into the moveFrequency
      if(moveFrequency.search(MoveInfo(itr.currentNode->data->setofMoves[totalMoves-1].move[1])))
      {
        moveFrequency.loc->data.occurence++; //increase occurence
      }
    }
  }

  auto highestOccurence = moveFrequency.root; //highest occuring node is the root node by default
  
  for(auto itr=moveFrequency.begin(); itr != moveFrequency.end(); itr++) //iterate over the list to get the most occurring node
  {
    if( itr.currentNode->data.occurence > highestOccurence->data.occurence  )
      highestOccurence = itr.currentNode; //update highest occurence node if its frequency is greater
  }

  cout << endl << "Most played move after " << pgn << " is: " << endl;
  cout << "Move: " << highestOccurence->data.move << endl;
  cout << "Times played: " << highestOccurence->data.occurence << endl;
}

int main() 
{
  AVLTree<EventDate> eventDateTree; // Stores all event's Date
  AVLTree<EventLocation> eventLocationTree; // Stores all Event's location
  AVLTree<Event> eventTree; // Stores all Events

  AVLTree<Player> playerTree; // Stores all player's data
  AVLTree<Game> gameTree; // Stores all the Game data
  AVLTree<ECO> ecoTree; // Stores all ECO data
  AVLTree<OpeningNode> openingsTree; // Stores all the Opening data -> Name and Variations

  //reading the whole stuff from the big file 

  cout << endl << "Loading the chess data set." << endl;

  std::ifstream pgnfile("./WorldCup2021.pgn"); //open data file
  pgn::GameCollection games;
  pgnfile >> games; //parse data file

  // now counting and printing the result
	std::cout << std::endl << "The dataset contains " << games.size() << " games." << std::endl << std::endl;

  // Stores all players that played at event
  HashTable<Player *> eventPlayers( games.size()/10 ) ;
  
  // Stores all the games of event
  HashTable<Game *> eventGames( games.size()/10 );
  
  // Stores games of player
  HashTable<Game *> playerGames( 1300 );
  
  // Stores all player matchups -> player played with other players
  HashTable<Player *> playerMatchups(1300);
  
  // Stores all pointer of event that player had played 
  HashTable<Event *> playerEvents(1300);
  
  std::ifstream chessDataset; //parses the pgndata.txt file for opening data
  chessDataset.open("pgndata.txt",std::ifstream::in);
  string oname; //temp variable for storing opening name
  string EcoCode; //tmep variable for storing ECO code
  int move_number;
  char dot;
  string whiteMove, blackMove, pgn;
  chessDataset.peek(); //simply checks the file stream pointer without advancing it
  while(chessDataset.good()) //loop over each line until EOF is reached
  {
    getline(chessDataset, EcoCode, '\t'); //get ecocode
    if( !ecoTree.search(ECO(EcoCode) ))
      ecoTree.insert(EcoCode); //insert into ecoTree

    getline(chessDataset, oname, '\t'); //get opening name
    getline(chessDataset, pgn); //get pgn of the opening

    int totalMoves = extractMaximum(pgn); //returns the number of moves played in the opening
    openingsTree.insert(OpeningNode{oname, totalMoves}); //as each opening is always unique, we need to insert it
    stringstream moveStream(pgn); //creates a string stream of opening pgn for move insertion purposes
    while(moveStream >> move_number >> dot >> whiteMove)
    {
      openingsTree.loc->data.setofMoves[move_number-1].moveNumber = move_number;
      openingsTree.loc->data.setofMoves[move_number-1].move[0] = whiteMove;
      if(moveStream >> blackMove)
        openingsTree.loc->data.setofMoves[move_number-1].move[1] = blackMove;
      else
        openingsTree.loc->data.setofMoves[move_number-1].move[1] = "";
    }
    ecoTree.loc->data.addOpening(openingsTree.loc->data); //add the opening to the ECO node
    openingsTree.loc->data.belongsToECO = &ecoTree.loc->data; //add a pointer of ECO to the opening that was just inserted
    chessDataset.peek(); //checks the state of the stream - whether we have reached EOF or not
  }
  chessDataset.close(); //close the file

  pgn::GameCollection::iterator itr; //define an interator for all the games in the data file
  bool makeNewEvent; //tells if new event node is created

  //temporary pointers used for insertion purposes later on
  Event* eventPtr = NULL;
  Player* whitePlayerSidePtr = NULL;
  Player* blackPlayerSidePtr = NULL;

  for (itr = games.begin(); itr != games.end(); itr++) //iterate through all the games in the file, and storing them
  {
    if(eventGames.getIndex( itr->tags()["Event"].value()) == -1 ) //search the hashTable of eventGames to see if the event exists
    //as an event cannot exist without game(s), we simply see if the current event being parsed has an empty hash index or not
    {
      makeNewEvent = true; //set makeNewEvent to true if an event does not already exist
    }
    else {
      makeNewEvent = false; //else set it to fast
    }
    
    if( !eventLocationTree.search( itr->tags()["Site"].value() ) )
    { //create a new location node if it does not exist
      eventLocationTree.insert(itr->tags()["Site"].value());
    }
    if(!eventDateTree.search(itr->tags()["EventDate"].value()))
    { //create a new date noded if it exists
      //EventDate *NewDate = new EventDate(itr->tags()["EventDate"].value());
      eventDateTree.insert(itr->tags()["EventDate"].value());
    }
    if(makeNewEvent) //if we are creating a new event node in the eventTree
    {
      eventTree.insert(Event(itr->tags()["Event"].value(), &eventLocationTree.loc->data, &eventDateTree.loc->data));
      eventPtr = &eventTree.loc->data; //save the address of the newly created event
    } 
    else //if the event node already exists in the list, we will need to save its address
    {
      eventPtr = eventGames.hashTable[eventGames.getIndex(itr->tags()["Event"].value())].list.root->data->playedInEvent;
    }

    if(!playerTree.search(itr->tags()["White"].value()))
    {
      playerTree.insert(itr->tags()["White"].value());
      whitePlayerSidePtr = &playerTree.loc->data;
    }
    else
    {
      whitePlayerSidePtr = &playerTree.loc->data;
    }

    if(!playerTree.search(itr->tags()["Black"].value()))
    {
      playerTree.insert(itr->tags()["Black"].value());
      blackPlayerSidePtr = &playerTree.loc->data;
    }
    else
    {
      blackPlayerSidePtr = &playerTree.loc->data;
    }
  
    //update hash table pointers
    //update players of event
    eventPlayers.doubleHashInsert( eventPtr->eventName, whitePlayerSidePtr); 
    eventPlayers.doubleHashInsert( eventPtr->eventName, blackPlayerSidePtr); 
    //update event of players
    playerEvents.doubleHashInsert(whitePlayerSidePtr->playerName, eventPtr);  
    playerEvents.doubleHashInsert(blackPlayerSidePtr->playerName, eventPtr); 

    pgn::Game game = *itr; //parses the current game
    pgn::MoveList moveList = game.moves(); //stores the list of moves in moveList

    ecoTree.search(itr->tags()["ECO"].value());
    
    pgn = moveList.getPGN(); //get the moves in PGN format

    Game *newGame = new Game(eventPtr, whitePlayerSidePtr, blackPlayerSidePtr, &ecoTree.loc->data, itr->tags()["Result"].value(),  stoi(itr->tags()["WhiteElo"].value()), stoi(itr->tags()["BlackElo"].value()), moveList.size(), pgn); //create a new game object

    //update player score based on game result
    if(itr->tags()["Result"].value() == "1-0")
    {
      whitePlayerSidePtr->wins++;
      blackPlayerSidePtr->losses++;
    }
    else if(itr->tags()["Result"].value() == "0-1")
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

    //update player matchups
    playerMatchups.doubleHashInsert(whitePlayerSidePtr->playerName, blackPlayerSidePtr);
    playerMatchups.doubleHashInsert(blackPlayerSidePtr->playerName, whitePlayerSidePtr);

    //update games of an event
    eventGames.doubleHashInsert(eventPtr->eventName, newGame);

    //update player games
    playerGames.doubleHashInsert(whitePlayerSidePtr->playerName, newGame);
    playerGames.doubleHashInsert(blackPlayerSidePtr->playerName, newGame);

    //inserts new game into the tree
    gameTree.insert(*newGame);
    gameTree.loc->data.playedInEvent = eventPtr; //stores pointer to event in the game
  } 
  
  int choice;
  string opening, userInput;
  do {
    cout << "\n\x1b[36mPlease select an action (-1 to end): " << endl << endl;
    cout << "\t1. Get Openings by ECO           2. Events Attended By A Player" << endl;
    cout << "\t3. Get Winrate of a Player       4. Get Player Matchups" << endl;
    cout << "\t5. Get Games Of An Event         6. Get Top Player Of Event" << endl;
    cout << "\t7. Print All Event Data          8. Pretty Print Events" << endl;
    cout << "\t9. Filter Events By Year         10. Get Total Games Played At Event" << endl;
    cout << "\t11. Filter Events By Location    12. Players That Attened Some Event" << endl;
    cout << "\t13. Get Variations of Opening    14. Get Score Between Two Players" << endl;
    cout << "\t15. Get All Games of a Matchup   16. Get All Games of a Player" << endl;
    cout << "\t17. Get Most Played Opening      18. Get Most Played Move After A Position" << endl;
    cout << "\t19. Filter Player Games By Year  20. Get Most Played Opening Of Event" << endl;
    cout << "\t21. Filter Games By Rating       22. Filter Games By Moves" << endl;
    cout << "\x1b[0m"; cin >> choice; cout << endl;
    switch(choice)  //switch statement to check user input and calling respective functions
    {
      case -1:
        break;
      
      // Get openings
      case 1:
      {
        cout << "\e[46mEnter an ECO Code:\x1b[0m "; cin >> opening;
        getOpenings(opening, ecoTree);
      }; break;

      // Events Attended by player
      case 2:
      {
        cout << "\e[46mEnter Player Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        printEventData(userInput, playerEvents);
      }; break;
      
      // win Rate
      case 3:
      {
        cout << "\e[46mEnter Player Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        auto searchp = Player(userInput);
        playerTree.search(searchp);
        playerTree.loc->data.getWinrate();
        cout << endl;
      }; break;
      
      // Get Player MatchUps
      case 4:
      {
        cout << "\e[46mEnter Player Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getPlayerMatchups(userInput, playerMatchups);
        cout << endl;
       }; break;

      // Get games of event
      case 5:
      {
        cout << "\e[46mEnter Event Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        getGamesOfEvent(userInput, eventGames);
        cout << endl;
      }; break;

      // Get Top Player Of Event
      case 6:
      {
        cout << "\e[46mEnter Event Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        getTopPlayerOfEvent(userInput, eventGames);
        cout << endl;
      }; break;

      // Prints All events
      case 7:
      {
        cout << "\e[0;32mFollowing tournaments are available in our database:\x1b[0m " << endl << endl;
        printAllEvents(eventTree.root);
      }; break;

      // Pretty Print Event List
      case 8:
      {
        eventTree.prettyPrint();
      }; break;

      // Get Events By year
      case 9:
      {
        cout << "\e[46mEnter year:\x1b[0m ";
        cin >> userInput;
        cout << endl;
        getEventsByYear(eventTree.root, stoi(userInput));
      }; break;

      // Get Total Games Played At Event
      case 10:
      {
        cout << "\e[46mEnter Event Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getTotalGamesPlayedAtEvent(userInput, eventGames);
        cout << endl;
      }; break;

      // Get Events Of Same Location
      case 11:
      {
        cout << "\e[46mEnter Location:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        getEventsOfSameLocation(eventTree.root, userInput);
      }; break;

      // Players that attened certain event
      case 12:
      {
        cout << "\e[46mEnter Event Name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        printPlayerData (userInput, eventPlayers);
      }; break;

      //Displays variations of an opening
      case 13:
      {
        cout << "\e[46mEnter an opening name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << endl;
        getVariationsOfOpenings(userInput, openingsTree.root);
      }; break;

      //Gets lifetime score between 2 players
      case 14:
      {
        string player2;
        cout << "\e[46mEnter player 1's name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << "\e[46mEnter player 2's name:\x1b[0m ";
        getline(cin, player2);
        cout << endl;
        getMatchupScore(userInput, player2, playerGames);
      }; break;
  
      //Gets lifetime games between 2 players
      case 15:
      {
        string player2;
        cout << "\e[46mEnter player 1's name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << "\e[46mEnter player 2's name:\x1b[0m ";
        getline(cin, player2);
        cout << endl;
        getMatchupGames(userInput, player2, playerGames);
      }; break;

      //Gets lifetime games of a player
      case 16:
      {
        cout << "\e[46mEnter player's name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        printGameData(userInput, playerGames);
      }; break;

      //Prints the most played opening by a player
      case 17:
      {
        cout << "\e[46mEnter player's name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getMostPlayedOpening(userInput, playerGames);
      }; break;
    
      //given a chess position, the program outputs the most played move
      case 18:
      {
        cout << "\e[46mEnter PGN:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getMostPlayedMove(userInput, gameTree.root);
      }; break;

      //prints all games of a player played in a certain year
      case 19:
      {
        int year;
        cout << "\e[46mEnter player's name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        cout << "\e[46mEnter year:\x1b[0m ";
        cin >> year;
        cout << endl;
        getPlayerGamesByYear(userInput, year, playerGames);
      }; break;

      case 20:
      {
        cout << "\e[46mEnter event name:\x1b[0m ";
        cin.ignore();
        getline(cin, userInput);
        getMostPlayedOpening(userInput, eventGames);
      }; break;

      case 21:
      {
        cout << "\e[46mEnter lower bound for average rating:\x1b[0m ";
        cin >> userInput;
        cout << endl;
        filterGamesByRating(stoi(userInput), gameTree.root);
      }; break;

      case 22:
      {
        cout << "\e[46mEnter upper bound for total moves:\x1b[0m ";
        cin >> userInput;
        cout << endl;
        filterGamesByMoves(stoi(userInput), gameTree.root);
      }; break;
  }
  } while (choice != -1);
}