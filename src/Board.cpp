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

FieldState Board::getWinner()
{
  FieldState clr;
  //Corners
  clr = m_fields[0][0];
  if (clr!= FIELD_EMPTY &&
      m_fields[0][0] == clr &&
      m_fields[0][3] == clr &&
      m_fields[3][0] == clr &&
      m_fields[3][3] == clr)
    {
      return clr;
    }

  //Quarters
  for(int i = 0; i < SIZE; i+=2)
    {
      for(int j = 0; j < SIZE; j+=2)
	{
	  clr = m_fields[i][j];
	  if(clr != FIELD_EMPTY &&
	     m_fields[i][j] == clr &&
	     m_fields[i][j+1] == clr &&
	     m_fields[i+1][j] == clr &&
	     m_fields[i+1][j+1] == clr)
	    {
	      return clr;
	    }
	}
    }

  //Horizontal lines
  for(int i = 0; i < SIZE; i++)
    {
      FieldState currentColor = m_fields[i][0];
      if (currentColor == FIELD_EMPTY)
	continue;
      
      int j = 1;
      for(; j < SIZE; j++)
	if (currentColor != m_fields[i][j])
	  continue;
      
      if (j == SIZE)
	return currentColor;
    }

  //Vertical lines
  for(int j = 0; j < SIZE; j++)
    {
      FieldState currentColor = m_fields[0][j];
      if (currentColor == FIELD_EMPTY)
	continue;
      
      int i = 1;
      for(; i < SIZE; i++)
	if (currentColor != m_fields[i][j])
	  continue;
      
      if (i == SIZE)
	return currentColor;
    }

  return FIELD_EMPTY;
}

bool Board::isTerminal()
{
  return getWinner() != FIELD_EMPTY;
}
