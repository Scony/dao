#include "State.h"

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
  
  //TODO: Implement from->to

  if (m_current == FIELD_PLAYER1)
    next.m_current = FIELD_PLAYER2;
  else
    next.m_current = FIELD_PLAYER1;

  return next;
}
