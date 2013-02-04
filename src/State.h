#ifndef STATE_H
#define STATE_H

#include <iostream>
#include"Board.h"

typedef unsigned int dao_hash_short;
typedef unsigned int dao_hash_invariant;

class Move;

class State
{
public:
  static const char AXIS_N = 8;
  static const char POSITION_MAP[AXIS_N][16];
  static const char INVERSE_POSITION_MAP[AXIS_N][16];

  Board m_board;
  FieldState m_current;

  //TODO: Add copy constructors
  State();
  State(FieldState current_move, Board board = Board::initialBoard());
  ~State();

  State move(const Move& m) const;

  dao_hash_short getHash() const;
  dao_hash_invariant getInvariantHash(int* axis_ptr=NULL) const;

  void print() const;
  std::ostream& write(std::ostream& out) const;
  std::istream& read(std::istream& in);
  
};
#endif
