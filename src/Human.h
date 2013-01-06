#ifndef HUMAN_H
#define HUMAN_H

#include "Player.h"
#include "State.h"
#include "Move.h"

class GraphicalBoard;

class Human : public Player
{
public:
  Human(const PlayerConfiguration& config, GraphicalBoard* gBoard);
  ~Human();

  virtual void proposeMove(State state);
  virtual bool isInteractive() const;

  void commitMoveProposal(Move m);
private:
  GraphicalBoard* m_gBoard;
};

#endif
