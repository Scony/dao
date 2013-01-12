#include "State.h"
#include "Move.h"

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
  
  next.m_board.m_fields[m.from%4][m.from/4] = FIELD_EMPTY;
  next.m_board.m_fields[m.to%4][m.to/4] = m_board.m_fields[m.from%4][m.from/4];

  if (m_current == FIELD_PLAYER1)
    next.m_current = FIELD_PLAYER2;
  else
    next.m_current = FIELD_PLAYER1;

  return next;
}
