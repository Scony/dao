#ifndef MOVE_H
#define MOVE_H

class Move
{
  //params
 public:
  Move();
  ~Move();

  //Przechowuje następne stany
  int from, to;

  int nextStateHash;
  int nextStateInvariantHash;
  //State nextState;

  Move* next;
};

/*
class MoveSet
{
  State[16] moves;
  bool valid;
}
*/

/*
MoveSet m;
getAvailableMoves(m);
*/

#endif
