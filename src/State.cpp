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
  const FieldState* board_ptr =
    reinterpret_cast<const FieldState*>(m_board.m_fields);
  dao_hash_short hash = 0;

  for(int i = 0; i < m_board.SIZE * m_board.SIZE; i++)
    {
      hash <<= 2;

      switch (board_ptr[i])
	{
	case FIELD_PLAYER1:
	  hash |= 0x01;
	  break;
	case FIELD_PLAYER2:
	  hash |= 0x02;
	  break;
	default:
	  break;
	}
    }

  if (m_current == FIELD_PLAYER2)
    hash = ~hash;

  return hash;
}

void State::print() const
{
  for(int y = 0; y < m_board.SIZE; y++)
    {
      for(int x = 0; x < m_board.SIZE; x++)
	{
	  switch (m_board.m_fields[y][x]) 
	    {
	    case FIELD_EMPTY:
	      cout << " ";
	      break;
	    case FIELD_PLAYER1:
	      cout << "O";
	      break;
	    case FIELD_PLAYER2:
	      cout << "#";
	      break;
	    }
	}
      cout << endl;
    }
}

ostream& State::write(ostream& out) const
{
  const FieldState* board_ptr =
    reinterpret_cast<const FieldState*>(m_board.m_fields);

  out << static_cast<char>(static_cast<char>(m_current) + '%') << endl;
  for(int i = 0; i < m_board.SIZE * m_board.SIZE; i++)
    {
      out << static_cast<char>(static_cast<char>(board_ptr[i]) + '%');
      if (i % 4 == 3)
	out << endl;
    }

  return out;
}

istream& State::read(istream& in)
{
  return in;
}
