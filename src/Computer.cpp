#include <iostream>
#include <unistd.h>
#include "Computer.h"
#include "DaoException.h"

using namespace std;

AIStrategy::AIStrategy(const Game* game,
		       const PlayerConfiguration& config) 
{
  m_game = game;
  m_cancelRequest = false;
}

void AIStrategy::startTiming()
{
  clock_gettime(CLOCK_MONOTONIC, &m_startTime);
}

void AIStrategy::endTiming()
{
  timespec endTime;
  clock_gettime(CLOCK_MONOTONIC, &endTime);

  double st = m_startTime.tv_sec + m_startTime.tv_nsec * 1e-9;
  double en = endTime.tv_sec + endTime.tv_nsec * 1e-9;
  double elapsed = en - st;
  cout << "---ELAPSED TIME" << elapsed << endl;
}

void AIStrategy::cancel()
{
  m_cancelRequest = true;
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
  if (m_thread)
    {
      if (m_strategy)
	m_strategy->cancel();
      m_thread->join();
    }
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

