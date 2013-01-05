#ifndef DAO_EXCEPTION_H
#define DAO_EXCEPTION_H

#include <stdexcept>

class DaoException: public std::runtime_error
{
public:
  DaoException(const std::string& s)
    : std::runtime_error(s) {};
};

#endif
