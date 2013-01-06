#ifndef HUMAN_H
#define HUMAN_H

#include "Player.h"
#include "State.h"

class Human : public Player
{
public:
  Human(const PlayerConfiguration& config, GraphicalBoard* gBoard);
  ~Human();

  virtual void proposeMove(State state);
  virtual bool isInteractive() const;
};

#endif
