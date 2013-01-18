#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "State.h"
#include "Configuration.h"

class Heuristic
{
 protected:
  const PlayerConfiguration & m_config;
 public:
  Heuristic(const PlayerConfiguration & config);
  virtual ~Heuristic();
  virtual int eval(State * state) = 0;
};

class LBHeuristic : public Heuristic
{
 public:
  LBHeuristic(const PlayerConfiguration & config);
  ~LBHeuristic();
  int eval(State * state);
};

#endif
