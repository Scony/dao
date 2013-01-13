#include "Computer.h"
#include <iostream>

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

  Move * pMove = m_moveSet.begin();
  int moves = 0;
  while(pMove != NULL)
    {
      pMove = pMove->next;
      moves++;
    }

  int move = rand() % moves;
  cout << "randed: " << move << endl;
  pMove = m_moveSet.begin();
  while(move--)
    pMove = pMove->next;

  m_proposedMove = *pMove;

  dispatcher_move_proposed.emit();
}

bool Random::isInteractive() const
{
  return false;
}
