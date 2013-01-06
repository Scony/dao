#include "Player.h"
#include "Configuration.h"
#include "Human.h"
#include "Computer.h"

Player::Player(const PlayerConfiguration& config)
{
  m_color = config.m_color;
  m_name = config.m_name;
}

Player::~Player()
{
}

GraphicalBoard* PlayerFactory::s_gBoard = 0;

Player* PlayerFactory::createPlayer(const PlayerConfiguration& config)
  throw(DaoException)
{
  switch (config.m_type)
    {
    case PLAYER_HUMAN:
      return new Human(config, s_gBoard);
    default:
      throw DaoException("Player type not implemented");
    }
}

void PlayerFactory::setGBoard(GraphicalBoard* board)
{
  s_gBoard = board;
}

  
