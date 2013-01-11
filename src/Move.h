#ifndef MOVE_H
#define MOVE_H

struct Move
{
  //Przechowuje nastepne stany
  int from;
  int to;

  int nextStateHash;
  int nextStateInvariantHash;
  //State nextState;

  Move * next;

  Move();
  Move(Move * next);
  ~Move();
};


struct MoveSet
{
  Move moves[32];
  bool valid;

  MoveSet();
  ~MoveSet();

  Move * begin();
};

#endif
