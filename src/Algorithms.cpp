#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include "Algorithms.h"

using namespace std;

Random::Random(const Game* game, const PlayerConfiguration& config)
  : AIStrategy(game, config)
{
}

void Random::run()
{
  m_game->getAvailableMoves(&m_moveSet,&m_state);

  cout << "Number of cycles before " << m_moveSet.size();  
  m_game->filterCycles(&m_moveSet);
  cout << " after " << m_moveSet.size() << endl;

  if (m_moveSet.size() == 0)
    {
      cout << "!!! Random player" << endl;
      cout << "!!! There is no move that will not cause cycles" << endl;
      cout << "!!! Breaking rules ;-)" << endl;

      m_game->getAvailableMoves(&m_moveSet, &m_state);
    }

  int moves_n = m_moveSet.size();
  if (moves_n == 0)
    throw DaoException("Ilość ruchów = 0");
  
  int move = rand() % moves_n;
  cout << "randed: " << move << endl;
  
  MoveSet::Iterator it = m_moveSet.begin();
  for(int i = 0; i < move; i++)
    {
      it = it.next();
    }

  m_proposedMove = it.at();

  int latency = Configuration::getInstance().m_latency;
  usleep(latency);
  dispatcher_move_proposed.emit();
}

HillClimber::HillClimber(const Game* game, const PlayerConfiguration& config)
  : AIStrategy(game, config)
{
  heuristic = new LBHeuristic(config);
}

HillClimber::~HillClimber()
{
  delete heuristic;
}

void HillClimber::run()
{
  m_game->getAvailableMoves(&m_moveSet,&m_state);

  int moves_n = m_moveSet.size();
  if (moves_n == 0)
    throw DaoException("Ilość ruchów = 0");

  int max = INT_MIN;
  MoveSet::Iterator best = m_moveSet.begin();


  for(MoveSet::Iterator it = best; it != m_moveSet.end(); it++)
    {
      State next = m_state.move(it.at());
      int rate = heuristic->eval(&next,m_state.m_current);
      cout << it.at().from << "->" << it.at().to << " H(): " << rate << endl;
      if(rate > max)
	{
	  max = rate;
	  best = it;
	}
    }

  m_proposedMove = best.at();
  cout << "best H(): " << max << endl;

  int latency = Configuration::getInstance().m_latency;
  usleep(latency);
  dispatcher_move_proposed.emit();
}

AlphaBeta::AlphaBeta(const Game* game,
		     const PlayerConfiguration& config)
  : AIStrategy(game, config)
{
  heuristic = new LBHeuristic(config);
  m_max_depth = 7;
}

AlphaBeta::~AlphaBeta()
{
  delete heuristic;
}

void AlphaBeta::run() 
{
  MoveSet moves;

  startTiming();
  m_game->getAvailableMoves(&moves, &m_state);
  m_game->filterCycles(&moves);
  if (moves.size() == 0)
    {
      cout << "!!! Alpha beta player" << endl;
      cout << "!!! There is no move that will not cause cycles" << endl;
      cout << "!!! Breaking rules ;-)" << endl;
      m_game->getAvailableMoves(&moves, &m_state);
    }

  MoveSet::Iterator it = moves.begin();
  int best = INT_MIN;
  Move bestMove;
  for(; it != moves.end(); it++)
    {
      State state = m_state.move(it.at());
      int val = alphaBeta(state, m_max_depth, AB_MIN,
			  INT_MIN, INT_MAX, m_state);
      if (val > best)
	{
	  bestMove = it.at();
	  best = val;
	}
    }

  m_proposedMove = bestMove;

  endTiming();
  int latency = Configuration::getInstance().m_latency;
  usleep(latency);
  dispatcher_move_proposed.emit();
}

int AlphaBeta::alphaBeta(const State& state, int depth, 
			 AlphaBeta::Type type,
			 int alpha, int beta,
			 const State& parent_state)
{
  FieldState currentPlayer = state.m_current;

  int heur_val = heuristic->eval(&state, currentPlayer);


  if (heur_val == heuristic->getMax() || 
      heur_val == heuristic->getMin() || 
      depth == 0)
    {
      //Terminal state for LBHeuristic or depht == 0
      if (type == AB_MIN)
	return -heur_val;
      else
	return heur_val;
    }
  
  MoveSet* moves = new MoveSet();
  m_game->getAvailableMoves(moves, &state);
  MoveSet::Iterator it = moves->begin();
  //TODO: Uwzglednic cykle

  int result;

  if (type == AB_MAX)
    {
      for(; it != moves->end(); it++)
	{
	  State next_state = state.move(it.at());
	  int val = alphaBeta(next_state, depth-1, AB_MIN, alpha, beta,
			      state);
	  alpha = val > alpha ? val : alpha;
	  if (alpha >= beta)
	    {
	      result = beta;
	      break;
	    }
	}
      if( it == moves->end())
	result = alpha;
    }
  else //type == AB_MIN
    {
      for(; it != moves->end(); it++)
	{
	  State next_state = state.move(it.at());
	  int val = alphaBeta(next_state, depth-1, AB_MAX, alpha, beta,
			      state);
	  beta = val < beta ? val : beta;
	  if (alpha >= beta)
	    {
	      result = alpha;
	      break;
	    }
	}
      if( it == moves->end())
	result = beta;
    }

  delete moves;
  return result;
}
