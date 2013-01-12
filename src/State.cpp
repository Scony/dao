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
