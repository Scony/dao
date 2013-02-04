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
      int rate = heuristic->eval(next,m_state.m_current);
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
  m_max_depth = config.m_depth;
}

AlphaBeta::~AlphaBeta()
{
  delete heuristic;
}

void AlphaBeta::run() 
{
  MoveSet moves;

  m_cancelRequest = false;
  m_maxPlayerColor = m_state.m_current;
  startTiming();
  m_visitedNodes = 0;
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
  cout << "NUMBER OF MOVES: " << moves.size() << endl;
  for(; it != moves.end(); it++)
    {
      if (m_cancelRequest)
	return;
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
  cout << "VISITED NODES: " << m_visitedNodes << endl;
  if (m_cancelRequest)
    return;
  int latency = Configuration::getInstance().m_latency;
  usleep(latency);
  dispatcher_move_proposed.emit();
}

int AlphaBeta::eval(const State& state, bool* isTerminalState)
{
  *isTerminalState = false;

  int heur_val = heuristic->eval(state, m_maxPlayerColor);
  if (heur_val == heuristic->getMax() || 
      heur_val == heuristic->getMin())
    {
      *isTerminalState = true;
    }

  return heur_val;
}

int AlphaBeta::alphaBeta(const State& state, int depth, 
			 AlphaBeta::Type type,
			 int alpha, int beta,
			 const State& parent_state)
{
  int heurVal;
  bool isTerminalState;
  int result;
  MoveSet* moves;

  if (m_cancelRequest)  //Anulowanie ruchu
    return 0;
  m_visitedNodes += 1;
 
  heurVal = eval(state, &isTerminalState);

  if( depth == 0 || isTerminalState)
    return heurVal;

  //  FieldState currentPlayer = state.m_current;
  
  moves = new MoveSet();
  m_game->getAvailableMoves(moves, &state);
  MoveSet::Iterator it = moves->begin();

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

AlphaBetaTT::AlphaBetaTT(const Game* game,
			 const PlayerConfiguration& config)
  : AlphaBeta(game, config)
{
}

AlphaBetaTT::~AlphaBetaTT()
{
}

int AlphaBetaTT::alphaBeta(const State& state, int depth, 
			 AlphaBeta::Type type,
			 int alpha, int beta,
			 const State& parent_state)
{
  int heurVal;
  bool isTerminalState;
  int result;
  MoveSet* moves;

  if (m_cancelRequest)  //Anulowanie ruchu
    return 0;
  m_visitedNodes += 1;

  //TODO: TTlookup
  //TODO: i ten kod z nastepnych slajdow

  heurVal = eval(state, &isTerminalState);

  if( depth == 0 || isTerminalState)
    return heurVal;

  //  FieldState currentPlayer = state.m_current;
  
  moves = new MoveSet();
  m_game->getAvailableMoves(moves, &state);
  MoveSet::Iterator it = moves->begin();
  MoveSet::Iterator best_move;

  if (type == AB_MAX)
    {
      int best = INT_MIN;

      for(; it != moves->end(); it++)
	{
	  State next_state = state.move(it.at());
	  int val = alphaBeta(next_state, depth-1, AB_MIN, alpha, beta,
			      state);
	  if (val > best)
	    {
	      best = val;
	      best_move = it;
	    }
	  if (best >= beta) break;
	  if (best > alpha) alpha = best;
	}
      result = best;
    }
  else //type == AB_MIN
    {
      int best = INT_MAX;

      for(; it != moves->end(); it++)
	{
	  State next_state = state.move(it.at());
	  int val = alphaBeta(next_state, depth-1, AB_MAX, alpha, beta,
			      state);
	  if (val < best)
	    {
	      best = val;
	      best_move = it;
	    }
	  if (best <= alpha) break;
	  if (best < beta) beta = best;
	}
      result = best;
    }

  //TODO: saveTT
  delete moves;
  return result;
}

void AlphaBetaTT::saveTT(TTEntry & entry)
{
  if (entry.m_heurVal <= entry.m_alpha)
    entry.m_bound = UPPER;
  else
    if(entry.m_heurVal >= entry.m_beta)
      entry.m_bound = LOWER;
    else
      entry.m_bound = ACCURATE;

  m_TT.insert(pair<dao_hash_invariant, TTEntry>(entry.m_hash,entry));
}

TTEntry * AlphaBetaTT::TTlookup(dao_hash_invariant hash)
{
  map<dao_hash_invariant, TTEntry>::iterator ptr = m_TT.find(hash);
  if(ptr == m_TT.end())
    return NULL;
  return &ptr->second;
}
