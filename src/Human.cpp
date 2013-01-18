#include <iostream>
#include "Human.h"
#include "GraphicalBoard.h"

using namespace std;

Human::Human(const PlayerConfiguration& config, GraphicalBoard* gBoard)
  : Player(config)
{
  cout << "Creating Human" << endl;
  m_gBoard = gBoard;
}

Human::~Human()
{
}

void Human::proposeMove(State state)
{
  currentState = state;

  m_gBoard->proposeMove(this);
}

bool Human::isInteractive() const
{
  return true;
}

MoveSet& Human::getAvailableMoves()
{
  Player::getAvailableMoves(&moveSet,&currentState);

  return moveSet;
}

void Human::commitMoveProposal(Move m)
{
  signal_move_proposed.emit(this, m);
}
