#include "State.h"
#include "Move.h"

#include <iostream>
using namespace std;

const char State::POSITION_MAP[State::AXIS_N][16] = 
  { {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {3, 7, 11, 15, 2, 6, 10, 14, 1, 5, 9, 13, 0, 4, 8, 12},
    {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
    {12, 8, 4, 0, 13, 9, 5, 1, 14, 10, 6, 2, 15, 11, 7, 3},
    {3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12},
    {12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3},
    {15, 11, 9, 3, 14, 10, 6, 2, 13, 9, 5, 1, 12, 8, 4, 0},
    {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15}
  };

const char State::INVERSE_POSITION_MAP[State::AXIS_N][16] = 
  { {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, //E
    {12, 8, 4, 0, 13, 9, 5, 1, 14, 10, 6, 2, 15, 11, 7, 3}, //R270
    {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}, //R180
    {3, 7, 11, 15, 2, 6, 10, 14, 1, 5, 9, 13, 0, 4, 8, 12}, //R90
    {3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12}, //S|
    {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15}, //S|R270
    {15, 11, 9, 3, 14, 10, 6, 2, 13, 9, 5, 1, 12, 8, 4, 0}, //S|R180
    {12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3}  //S|R90
  };

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

  for(int i = m_board.SIZE * m_board.SIZE - 1; i >= 0; i--)
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

dao_hash_invariant State::getInvariantHash(int* axis_ptr) const 
{
  int min_axis;
  const FieldState* board_ptr =
    reinterpret_cast<const FieldState*>(m_board.m_fields);
  
  unsigned int hashes[AXIS_N] = {0};
  unsigned int min_hash;

  for(int i = 0; i < m_board.SIZE * m_board.SIZE; i++)
    {
      unsigned int field_value = 0;
      switch(board_ptr[i])
	{
	case FIELD_PLAYER1: field_value = 0x1; break;
	case FIELD_PLAYER2: field_value = 0x2; break;
	default: break;
	}
      for(int axis = 0; axis < AXIS_N; axis++)
	{
	  hashes[axis] |= (field_value << (2 * POSITION_MAP[axis][i]));
	}
    }

  min_hash = hashes[0];
  min_axis = 0;
  for (int axis = 1; axis < AXIS_N; axis++)
    {
      if (hashes[axis] < min_hash)
	{
	  min_hash = hashes[axis];
	  min_axis = axis;
	}
    }

  if (m_current == FIELD_PLAYER2)
    min_hash = ~min_hash;

  if (axis_ptr != NULL)
    *axis_ptr = min_axis;

  return min_hash;
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
  FieldState* board_ptr =
    reinterpret_cast<FieldState*>(m_board.m_fields);

  char c;
  in >> c;
  m_current = (FieldState)(int)(c - '%');
  for(int i = 0; i < m_board.SIZE * m_board.SIZE; i++)
    {
      in >> c;
      board_ptr[i] = (FieldState)(int)(c - '%');
    }

  return in;
}
