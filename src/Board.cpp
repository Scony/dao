#include<string.h>

#include "Board.h"

Board::Board()
{
}

Board::Board(const Board& b)
{
  memcpy(m_fields, b.m_fields, SIZE * SIZE * sizeof(int));
}


Board::~Board()
{
}

Board Board::rotate180()
{
  Board b;
  for(int i = 0; i < SIZE; i++)
    {
      for(int j = 0; j < SIZE; j++)
	{
	  b.m_fields[i][j] = m_fields[SIZE-i-1][SIZE-j-1];
	}
    }
  return b;
}

Board Board::initialBoard()
{
  Board b;
  for(int i = 0; i < SIZE; i++)
    {
      for(int j = 0; j < SIZE; j++)
	{
	  if(i == j)
	    b.m_fields[i][j] = FIELD_PLAYER1;
	  else if (i+j == 3)
	    b.m_fields[i][j] = FIELD_PLAYER2;
	  else
	    b.m_fields[i][j] = FIELD_EMPTY;
	}
    }
  return b;
}

int Board::getWinner()
{
  return 1;
}

bool Board::isTerminal()
{
  return getWinner() >= 0;
}
