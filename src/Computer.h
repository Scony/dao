#ifndef COMPUTER_H
#define COMPUTER_H

#include <glibmm/dispatcher.h>
#include <glibmm/thread.h>

#include "Player.h"
#include "State.h"
#include "Configuration.h"
#include "Game.h"
#include "Move.h"

//TODO: Inherit all AI players from Computer class??

class Random : public Player
{
  MoveSet m_moveSet;
public:
  Random(const PlayerConfiguration& config, const Game* game=0);
  virtual ~Random();

  void proposeMove(State state);
  bool isInteractive() const;

  Glib::Dispatcher signal_done;

protected:
  Glib::Thread* worker_thread;
};
  
#endif
