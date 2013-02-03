#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Computer.h"

class Random : public AIStrategy
{
public:
  Random(const Game* game, const PlayerConfiguration& config);
  void run();

private:
  MoveSet m_moveSet;
};

class HillClimber : public AIStrategy
{
public:
  HillClimber(const Game* game, const PlayerConfiguration& config);
  ~HillClimber();
  void run();

private:
  MoveSet m_moveSet;
};

class AlphaBeta: public AIStrategy
{
public:
  AlphaBeta(const Game* game, const PlayerConfiguration& config);
  virtual ~AlphaBeta();
  void run();
protected:
  enum Type {
    AB_MIN,
    AB_MAX
  };
  int m_max_depth;
  int m_visitedNodes;
  FieldState m_maxPlayerColor;

  /**
   * Ewaluacja heurystyczna stanu planszy.
   * Efektem ubocznym jest obliczenie, czy wystapil stan koncowy
   */
  int eval(const State& state, bool* isTerminalState);
  virtual int alphaBeta(const State& state, int depth,
			AlphaBeta::Type type,
			int alpha, int beta,
			const State& parent_state);
};

enum TTEntryType
  {
    ACCURATE,
    LOWER,
    UPPER
  };
  
struct TTEntry
{
  dao_hash_invariant m_hash;
  int m_heurVal;
  int m_alpha, m_beta;
  TTEntryType m_type;
  int m_depth;
  Move m_bestMove;
};

class AlphaBetaTT: public AlphaBeta
{
public:
  AlphaBetaTT(const Game* game, const PlayerConfiguration& config);
  ~AlphaBetaTT();

protected:
  int alphaBeta(const State& state, int depth, AlphaBeta::Type type,
		int alpha, int beta, const State& parent_state);
  //map<dao_hash_invariant, TTEntry> m_TT;
  //saveTT
  //TTEntry* TTlookup; (moze byc NULL)
};

#endif
