#ifndef STATE_H
#define STATE_H

#include"Board.h"

typedef unsigned int dao_hash_short;
typedef unsigned int dao_hash_invariant;

class State
{
public:
  Board board;
  int current_move;

  State();
  ~State();

  dao_hash_short getHash() const;
  dao_hash_invariant getInvariantHash() const;
};

#endif
