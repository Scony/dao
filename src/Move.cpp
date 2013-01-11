#include "Move.h"

#include <iostream>

Move::Move()
{
  next = NULL;
}

Move::Move(Move * next)
{
  this->next = next;
}

Move::~Move()
{
}

MoveSet::MoveSet()
{
  //
}

MoveSet::~MoveSet()
{
  //
}

Move * MoveSet::begin()
{
  return &moves[0]; //moves;
}
