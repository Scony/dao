#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "State.h"
#include "Configuration.h"

class Heuristic
{
 protected:
  PlayerConfiguration & m_config;
 public:
  Heuristic(PlayerConfiguration & config);
  ~Heuristic();
  virtual int eval(State * state) = 0;
};

class LBHeuristic : public Heuristic
{
  LBHeuristic(PlayerConfiguration & config);
  ~LBHeuristic();
  int eval(State * state);
};

#endif
