#ifndef STATE_H
#define STATE_H

#include"Board.h"

typedef unsigned int dao_hash_short;
typedef unsigned int dao_hash_invariant;

class Move;

class State
{
public:
  Board m_board;
  FieldState m_current;

  //TODO: Add copy constructors
  State();
  State(FieldState current_move, Board board = Board::initialBoard());
  ~State();

  State move(const Move& m) const;

  dao_hash_short getHash() const;
  dao_hash_invariant getInvariantHash() const;
};

#endif
