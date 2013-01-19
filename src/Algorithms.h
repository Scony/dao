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
  ~AlphaBeta();
  void run();
private:
  enum Type {
    AB_MIN,
    AB_MAX
  };
  int m_max_depth;

  int alphaBeta(const State& state, int depth, AlphaBeta::Type type,
		int alpha, int beta,
		const State& parent_state);
};

#endif
