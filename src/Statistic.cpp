#include <iostream>
#include "Statistic.h"
using namespace std;

Statistic& Statistic::getInstance()
{
  static Statistic stat;
  return stat;
}

void Statistic::clear()
{
  for(int i = 0; i < ALGORITHM_NUM; i++)
    {
      m_nodeCount[i] = 0;
      m_moveCount[i] = 0;
    }
}

void Statistic::addNodeCount(int count, PlayerAlgorithm alg)
{
  if (m_moveCount[alg] == 0)
    {
      m_nodeCount[alg] += count;
      m_moveCount[alg] += 1;
    }
}

void Statistic::print()
{
  cout << "STATYSTYKA" << endl;
  for( int i = 0; i < ALGORITHM_NUM; i++)
    {
      cout << "alg: " << i << " nodes: " << m_nodeCount[i];
      cout << " moves: " << m_moveCount[i] << endl;
    }
}

