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

int LBHeuristic::eval(State * state)
{
  FieldState player = state->m_current;
  FieldState opponent = 1 - player;

  //parameters: k[i] - weight of i-th layer, h - hope
  int k[3];
  for(int i = 0; i < 3; i++)
    k[i] = m_config.m_k[i];
  int h = m_config.m_h;

  //H()
  int max = 18 * (k1 + k2 + k3);
  int min = -max;

  //r-ity and pure r-ity of layer i and player j
  int r[5][2] = { 0 };
  int p[5][2] = { 0 };

  Board & board = state->m_board;

  //ammout of stones that belongs to i-th owner (for concrete term-state)
  int stones[3] = { 0 };

  //corners
  for(int i = 0; i < 4; i+= 3)
    for(int j = 0; j < 4; j+= 3)
      stones[board.m_fields[i][j]]++;

  r[stones[player]][player]++;
  r[stones[opponent]][opponent]++;
  if(stones[player] > 0 && stones[opponent] == 0)
    p[stones[player]][player]++;
  if(stones[opponent] > 0 && stones[player] == 0)
    p[stones[opponent]][opponent]++;
	  

  //quarters
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      {
	for(int k = 0; k < 2; k++)
	  stones[k] = 0;

	for(int k = 0; k < 2; k++)
	  for(int l = 0; l < 2; l++)
	    stones[board.m_fields[i+k][j+l]]++;

	r[stones[player]][player]++;
	r[stones[opponent]][opponent]++;
	if(stones[player] > 0 && stones[opponent] == 0)
	  p[stones[player]][player]++;
	if(stones[opponent] > 0 && stones[player] == 0)
	  p[stones[opponent]][opponent]++;
      }

  //horizontal lines
  for(int i = 0; i < 4; i++)
    {
      for(int k = 0; k < 2; k++)
	stones[k] = 0;

      for(int j = 0; j < 4; j++)
	stones[board.m_fields[i][j]]++;

      r[stones[player]][player]++;
      r[stones[opponent]][opponent]++;
      if(stones[player] > 0 && stones[opponent] == 0)
	p[stones[player]][player]++;
      if(stones[opponent] > 0 && stones[player] == 0)
	p[stones[opponent]][opponent]++;
    }

  //vertical lines
  for(int j = 0; j < 4; j++)
    {
      for(int k = 0; k < 2; k++)
	stones[k] = 0;

      for(int i = 0; i < 4; i++)
	stones[board.m_fields[i][j]]++;

      r[stones[player]][player]++;
      r[stones[opponent]][opponent]++;
      if(stones[player] > 0 && stones[opponent] == 0)
	p[stones[player]][player]++;
      if(stones[opponent] > 0 && stones[player] == 0)
	p[stones[opponent]][opponent]++;
    }

  if(p[4][player] > 0)
    return max;

  if(p[4][opponent] > 0)
    return min;

  int player_stable = p[1][player] * k[0] + p[2][player] * k[1] + p[3][player] * k[2];
  int player_hope = h * ((r[1][player] - p[1][player]) * k[0] +
			 (r[2][player] - p[2][player]) * k[1] +
			 (r[3][player] - p[3][player]) * k[2]);

  int opponent_stable = p[1][opponent] * k[0] + p[2][opponent] * k[1] + p[3][opponent] * k[2];
  int opponent_hope = h * ((r[1][opponent] - p[1][opponent]) * k[0] +
			   (r[2][opponent] - p[2][opponent]) * k[1] +
			   (r[3][opponent] - p[3][opponent]) * k[2]);

  return player_stable + player_hope - opponent_stable;
}
