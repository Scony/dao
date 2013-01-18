#include "Heuristic.h"

Heuristic::Heuristic(PlayerConfiguration & config)
{
  m_config = config;
}

Heuristic::~Heuristic()
{
}

LBHeuristic::LBHeuristic(PlayerConfiguration & config) : Heuristic(config)
{
}

LBHeuristic::~LBHeuristic()
{
}

LBHeuristic::eval(State * state)
{
  FieldState player = state->m_current;
  FieldState opponent = 1 - player;

  int k1 = 0;
  int k2 = 0;
  int k3 = 0;
  int h = 0;

  int r1[2] = { 0 };
  int r2[2] = { 0 };
  int r3[2] = { 0 };

  int p1[2] = { 0 };
  int p2[2] = { 0 };
  int p3[2] = { 0 };

  Board & board = state->m_board;

  //Corners
  for(int i = 0; i < 4; i+= 3)
    for(int j = 0; j < 4; j+= 3)
      {
	if(board.m_fields[i][j] == player)
	  

  //Quarters
  for(int i = 0; i < SIZE-1; i++)
    {
      for(int j = 0; j < SIZE-1; j++)
	{
	  clr = m_fields[i][j];
	  if(clr != FIELD_EMPTY &&
	     m_fields[i][j] == clr &&
	     m_fields[i][j+1] == clr &&
	     m_fields[i+1][j] == clr &&
	     m_fields[i+1][j+1] == clr)
	    {
	      cout << "Quarters" << endl;
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
	  break;
      
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
	  break;
      
      if (i == SIZE)
	return currentColor;
    }
}
