#include <iostream>
#include "Computer.h"
#include "DaoException.h"

using namespace std;

Random::Random(const PlayerConfiguration& config, const Game* game) : Player(config)
{
  cout << "Creating random computer" << endl;
}
Random::~Random()
{
}

void Random::proposeMove(State state)
{
  Player::getAvailableMoves(&m_moveSet,&state);

  int moves_n = m_moveSet.size();
  if (moves_n == 0)
    throw DaoException("Ilość ruchów = 0");

  int move = rand() % moves_n;
  cout << "randed: " << move << endl;
  
  MoveSet::Iterator it = m_moveSet.begin();
  for(int i = 0; i < move; i++)
    it = it.next();

  m_proposedMove = it.at();
  dispatcher_move_proposed.emit();
}

bool Random::isInteractive() const
{
  return false;
}
