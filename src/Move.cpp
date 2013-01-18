#include "Move.h"

#include <iostream>

using namespace std;

Move::Move()
{
  next = NULL;
}

Move::Move(int from, int to)
{
  this->from = from;
  this->to = to;
}

Move::~Move()
{
}


Move& MoveSet::Iterator::at()
{
  if (m_self == 0)
    throw DaoException("Invalid move dereferenced");
  return *m_self;
}

MoveSet::Iterator MoveSet::Iterator::next()
{
  MoveSet::Iterator it;

  if (it.m_self == 0)
    throw DaoException("Calling next on invalid MoveSet::Iterator");

  it.m_prev = m_self;
  it.m_self = m_self->next;

  return it;
}

bool MoveSet::Iterator::operator==(const MoveSet::Iterator& r)
{
  return (m_self == r.m_self);
}

bool MoveSet::Iterator::operator!=(const MoveSet::Iterator& r)
{
  return (m_self != r.m_self);
}

MoveSet::Iterator MoveSet::Iterator::operator++(int unused)
{
  if(m_self == 0)
    throw DaoException("Iterating over the range");
  
  m_prev = m_self;
  m_self = m_self->next;
  return *this;
}

MoveSet::MoveSet()
{
  clear();
}

MoveSet::~MoveSet()
{
  //
}

MoveSet::Iterator MoveSet::begin()
{
  MoveSet::Iterator it;
  it.m_prev = 0;
  it.m_self = m_head;
  return it;
}

MoveSet::Iterator MoveSet::end()
{
  MoveSet::Iterator it;
  it.m_prev = m_tail;
  it.m_self = 0;
  return it;
}

int MoveSet::size()
{
  return m_size;
}

void MoveSet::add(const Move& move)
  throw(DaoException)
{
  if (m_size >= 32)
    {
      throw DaoException("Adding too much elements to MoveSet");
    }

  if (m_head == 0)
    {
      moves[0] = move;
      m_head = moves;
      m_tail = moves;
    }
  else
    {
      Move* ptr = m_tail + 1;
      (*ptr) = move;
      m_tail->next = ptr;
      m_tail = ptr;
    }

  m_size += 1;
}

void MoveSet::clear()
{
  m_size = 0;
  m_head = 0;
  m_tail = 0;
}

MoveSet::Iterator MoveSet::remove(MoveSet::Iterator& it)
{
  if (it.m_self == 0)
    {
      throw DaoException("Deleting invalid iterator");
    }
  
  Iterator next = it.next();
  
  if (it.m_self == m_head)
    {
      m_head = next.m_self;
      next.m_prev = 0;
    }
  else
    {
      it.m_prev->next = next.m_self;
      next.m_prev = it.m_prev;
    }
  
  if( m_tail == it.m_self)
    m_tail = it.m_prev;
  
  it.m_self = 0;
  m_size--;

  return next;
}
