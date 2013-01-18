#include <iostream>
#include <unistd.h>
#include "Computer.h"
#include "DaoException.h"

using namespace std;

AIStrategy::AIStrategy(const Game* game,
		       const PlayerConfiguration& config) 
{
  m_game = game;
}

Computer::Computer(const PlayerConfiguration& config,
		   const Game* game,
		   AIStrategy* strategy): Player(config)
{
  m_thread = 0;
  m_strategy = strategy;

  m_strategy->dispatcher_move_proposed.connect(
      sigc::mem_fun(*this, &Computer::onDispatcherMoveProposed ));
}

Computer::~Computer()
{
}

void Computer::proposeMove(State state)
{
  m_strategy->m_state = state;
  m_thread =
    Glib::Thread::create(sigc::mem_fun(*m_strategy, &AIStrategy::run));
}

bool Computer::isInteractive() const
{
  return false;
}

void Computer::onDispatcherMoveProposed()
{
  m_thread->join();
  m_thread = 0;
  signal_move_proposed.emit(this, m_strategy->m_proposedMove);
}

Random::Random(const Game* game, const PlayerConfiguration& config)
  : AIStrategy(game, config)
{
}

void Random::run()
{
  m_game->getAvailableMoves(&m_moveSet,&m_state);
  m_game->filterCycles(&m_moveSet);

  if (m_moveSet.size() == 0)
    {
      cout << "!!! Random player" << endl;
      cout << "!!! There is no move that will not cause cycles" << endl;
      cout << "!!! Breaking rules ;-)" << endl;

      m_game->getAvailableMoves(&m_moveSet, &m_state);
    }

  int moves_n = m_moveSet.size();
  if (moves_n == 0)
    throw DaoException("Ilość ruchów = 0");

  int move = rand() % moves_n;
  cout << "randed: " << move << endl;
  
  MoveSet::Iterator it = m_moveSet.begin();
  for(int i = 0; i < move; i++)
    it = it.next();

  m_proposedMove = it.at();

  sleep(1);
  dispatcher_move_proposed.emit();
}

