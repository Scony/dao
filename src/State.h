#ifndef STATE_H
#define STATE_H

#include"Board.h"

typedef unsigned int dao_hash_short;
typedef unsigned int dao_hash_invariant;

class State
{
public:
  Board m_board;
  int m_current_move;

  //TODO: Add copy constructors
  State();
  State(int current_move, Board board = Board::initialBoard());
  ~State();

  dao_hash_short getHash() const;
  dao_hash_invariant getInvariantHash() const;
};

#endif
