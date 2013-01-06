#include "State.h"

State::State()
{
}

State::State(int current_move, Board board)
{
  m_board = board;
  m_current_move = current_move;
}

State::~State()
{
}
