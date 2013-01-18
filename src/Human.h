#ifndef HUMAN_H
#define HUMAN_H

#include "Player.h"
#include "State.h"
#include "Move.h"

class GraphicalBoard;

class Human : public Player
{
  GraphicalBoard* m_gBoard;
  MoveSet moveSet;
  State currentState;

public:
  Human(const PlayerConfiguration& config, GraphicalBoard* gBoard);
  ~Human();

  virtual void proposeMove(State state);
  virtual bool isInteractive() const;

  MoveSet& getAvailableMoves();
  void commitMoveProposal(Move m);
};

#endif
