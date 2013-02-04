#ifndef STATISTIC_H
#define STATISTIC_H

#include "Player.h"

class Statistic
{
 public:
  static Statistic& getInstance();

  void clear();
  void addNodeCount(int count, PlayerAlgorithm alg);
  void print();

 private:
  int m_nodeCount[ALGORITHM_NUM];
  int m_moveCount[ALGORITHM_NUM];
};

#endif
