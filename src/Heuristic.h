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
  virtual int eval(const State & state, FieldState player) = 0;

  virtual int getMax() const = 0;
  virtual int getMin() const = 0;
};

class LBHeuristic : public Heuristic
{
 public:
  LBHeuristic(const PlayerConfiguration & config);
  ~LBHeuristic();
  int eval(const State & state, FieldState player);

  int getMax() const;
  int getMin() const;
};

#endif
