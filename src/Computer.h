#ifndef COMPUTER_H
#define COMPUTER_H

#include <glibmm/dispatcher.h>
#include <glibmm/thread.h>

#include "Player.h"
#include "State.h"
#include "Configuration.h"
#include "Game.h"
#include "Move.h"
#include "Heuristic.h"

class AIStrategy
{
public:
  State m_state;
  Move m_proposedMove;
  Heuristic * heuristic;

  AIStrategy(const Game* game, const PlayerConfiguration& config);
  virtual void run() = 0;
  
  Glib::Dispatcher dispatcher_move_proposed;
protected:
  const Game* m_game;
};


class Computer : public Player
{
public:
  Computer(const PlayerConfiguration& config, const Game* game,
	   AIStrategy* strategy);
  virtual ~Computer();

  void proposeMove(State state);
  bool isInteractive() const;

protected:
  Glib::Thread* m_thread;
  AIStrategy* m_strategy;
  
  void onDispatcherMoveProposed();

  friend class AIStrategy;
};


#endif
