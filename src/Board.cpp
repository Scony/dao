#include<string.h>

#include "Board.h"

Board::Board()
{
}

Board::Board(const Board& b)
{
  memcpy(m_board, b.m_board, SIZE * SIZE * sizeof(int));
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
	  b.m_board[i][j] = m_board[SIZE-i-1][SIZE-j-1];
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
	    b.m_board[i][j] = FIELD_STATE_COLOR_0;
	  else if (i+j == 3)
	    b.m_board[i][j] = FIELD_STATE_COLOR_1;
	  else
	    b.m_board[i][j] = FIELD_STATE_EMPTY;
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
