#include "State.h"
#include "Move.h"

#include <iostream>
using namespace std;

State::State()
{
}

State::State(FieldState current_move, Board board)
{
  m_board = board;
  m_current = current_move;
}

State::~State()
{
}

State State::move(const Move& m) const
{
  State next(*this);
  FieldState* board_ptr =
    reinterpret_cast<FieldState*>(next.m_board.m_fields);

  if (board_ptr[m.to] != FIELD_EMPTY)
    throw DaoException("Moving to non-empty field");
  if (board_ptr[m.from] == FIELD_EMPTY)
    throw DaoException("Moving from empty field");

  board_ptr[m.to] = board_ptr[m.from];
  board_ptr[m.from] = FIELD_EMPTY;
  
  if (m_current == FIELD_PLAYER1)
    next.m_current = FIELD_PLAYER2;
  else
    next.m_current = FIELD_PLAYER1;

  return next;
}

dao_hash_short State::getHash() const
{
  return 0;
}
