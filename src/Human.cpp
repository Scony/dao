#include <iostream>
#include "Human.h"

using namespace std;

Human::Human(const PlayerConfiguration& config, GraphicalBoard* gBoard)
  : Player(config)
{
  cout << "Creating Human" << endl;
}

Human::~Human()
{
}

void Human::proposeMove(State state)
{
  //TODO: implement
}
