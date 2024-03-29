#ifndef MOVE_H
#define MOVE_H

#include "DaoException.h"

class MoveSet;

struct Move
{
  //Przechowuje nastepne stany
  int from;
  int to;

  int nextStateHash;
  int nextStateInvariantHash;

  Move();
  Move(int from, int to);
  ~Move();

  Move transform(int axis) const;
  Move inverseTransform(int axis) const;

private:
  Move * next;
  friend class MoveSet;
};


class MoveSet
{
public:
  class Iterator
  {
  public:
    Move& at();
    MoveSet::Iterator next();
    
    bool operator==(const MoveSet::Iterator& r);
    bool operator!=(const MoveSet::Iterator& r);
    MoveSet::Iterator operator++(int unused);
    bool valid() {
      return m_self != NULL;
    }
  private:
    Move* m_prev;
    Move* m_self;
    
    friend class MoveSet;
  };
    
  MoveSet();
  ~MoveSet();

  MoveSet::Iterator begin();
  MoveSet::Iterator end();
  int size();

  void add(const Move& move)
    throw(DaoException);

  void clear();
  MoveSet::Iterator remove(MoveSet::Iterator& it);
  //TODO: Implement
  void setAsFirstMove(int from, int to);
  
private:
  Move moves[32];
  Move* m_head;
  Move* m_tail;
  int m_size;
};

#endif
